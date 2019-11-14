#include "InterThreadCommunicationMgr.h"

#include <iostream>
#include <algorithm>
#include <iterator>

namespace Thread
{

   InterThreadCommunicationMgr::InterThreadCommunicationMgr()
   {
      m_renderThreadJobs.reserve(mPoolSize);
      m_gameThreadJobs.reserve(mPoolSize);

      m_renderThreadJobsHashes.reserve(mPoolSize);
      m_gameThreadJobsHashes.reserve(mPoolSize);
   }

   InterThreadCommunicationMgr::~InterThreadCommunicationMgr()
   {
   }

   void InterThreadCommunicationMgr::PushGameThreadJob(const EnqueueJobPolicy policy, const Job& job)
   {
      ProcessPushGameThreadJob(policy, job);
   }

   void InterThreadCommunicationMgr::PushRenderThreadJob(const EnqueueJobPolicy policy, const Job& job)
   {
      ProcessPushRenderThreadJob(policy, job);
   }

   void InterThreadCommunicationMgr::ProcessPushRenderThreadJob(const EnqueueJobPolicy policy, const Job& job)
   {
      ProcessPushJob(policy, job, m_renderThreadJobs, m_renderThreadJobsHashes, m_renderThreadMutex);
   }

   void InterThreadCommunicationMgr::ProcessPushGameThreadJob(const EnqueueJobPolicy policy, const Job& job)
   {
      ProcessPushJob(policy, job, m_gameThreadJobs, m_gameThreadJobsHashes, m_gameThreadMutex);
   }

   void InterThreadCommunicationMgr::CORE_ReplaceJob(const Job& job, std::vector<Job>& jobs, threadJobsHashType& hashes)
   {
      const uint64_t jobHash = job.GetHash();
      const size_t index = *(hashes[jobHash].begin());
      std::vector<Job>::iterator replaceItemIt = jobs.begin() + index;
      *(replaceItemIt) = job;
   }

   void InterThreadCommunicationMgr::ProcessPushJob(const EnqueueJobPolicy policy, const Job& job, std::vector<Job>& jobs, threadJobsHashType& hashes, std::mutex& lockMutex)
   {
      const uint64_t jobHash = job.GetHash();

      const bool bJobExistsInPool = hashes[jobHash].size() > 0;

      if (bJobExistsInPool) // already exists in the pool
      {
         if (jobs.size() >= mPoolSize)
         {
            switch (policy)
            {
               case EnqueueJobPolicy::PUSH_ANYWAY:
               case EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH:
               {
                  std::lock_guard<std::mutex> lock(lockMutex);
                  CORE_ReplaceJob(job, jobs, hashes);
                  break;
               }
            }
         }
         else
         {
            switch (policy)
            {
               case EnqueueJobPolicy::PUSH_ANYWAY:
               {
                  std::lock_guard<std::mutex> lock(lockMutex);
                  hashes[jobHash].emplace(jobs.size());
                  jobs.emplace_back(job);
                  break;
               }
               case EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH:
               {
                  std::lock_guard<std::mutex> lock(lockMutex);
                  CORE_ReplaceJob(job, jobs, hashes);
                  break;
               }
            }
         }
      }
      else
      {
         std::lock_guard<std::mutex> lock(lockMutex);
         hashes.emplace(jobHash, threadIndex_t{ jobs.size() });
         jobs.emplace_back(job);
      }
   }

   void InterThreadCommunicationMgr::SpinGameThreadJobs()
   {
      std::lock_guard<std::mutex> lock(m_gameThreadMutex);

      size_t currentIndex = 0;
      while (AreGameJobsAwaiting())
      {
         auto jobIt = m_gameThreadJobs.begin();
         (*jobIt)();

         m_gameThreadJobsHashes[jobIt->GetHash()].erase(currentIndex);
         m_gameThreadJobs.erase(jobIt);

         ++currentIndex;
      }
   }

   void InterThreadCommunicationMgr::SpinRenderThreadJobs()
   {
      std::lock_guard<std::mutex> lock(m_renderThreadMutex);

      size_t currentIndex = 0;
      while (AreRenderJobsAwaiting())
      {
         auto jobIt = m_renderThreadJobs.begin();
         (*jobIt)();

         auto& item = m_renderThreadJobsHashes[jobIt->GetHash()];
         item.erase(currentIndex);
         m_renderThreadJobs.erase(jobIt);

         ++currentIndex;
      }
   }
}
