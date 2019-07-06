#pragma once

#include <functional>
#include <queue>
#include <mutex>

namespace Thread
{

#define ENQUEUE_RENDER_THREAD_JOB(InterThreadMgrInstance, Policy, JOB) \
InterThreadMgrInstance.PushRenderThreadJob(Policy, JOB)

#define ENQUEUE_GAME_THREAD_JOB(InterThreadMgrInstance, Policy, JOB) \
InterThreadMgrInstance.PushGameThreadJob(Policy, JOB)

#define SPIN_RENDER_THREAD_JOBS(InterThreadMgrInstance) \
InterThreadMgrInstance.SpinRenderThreadJobs();

#define SPIN_GAME_THREAD_JOBS(InterThreadMgrInstance) \
InterThreadMgrInstance.SpinGameThreadJobs();

   enum class EnqueueJobPolicy
   {
      IF_DUPLICATE_NO_PUSH,
      IF_DUPLICATE_REPLACE_AND_PUSH,
      PUSH_ANYWAY
   };

   class InterThreadCommunicationMgr
   {

      std::mutex m_gameThreadMutex;

      std::mutex m_renderThreadMutex;

      std::queue<std::function<void(void)>> m_gameThreadJobs;

      std::queue<std::function<void(void)>> m_renderThreadJobs;

   public:

      InterThreadCommunicationMgr();

      ~InterThreadCommunicationMgr();

      void PushGameThreadJob(const EnqueueJobPolicy, const std::function<void(void)>& job);

      void PushRenderThreadJob(const EnqueueJobPolicy, const std::function<void(void)>& job);

      /* @ Should be executed only on game thread! */
      void SpinGameThreadJobs();

      /* @ Should be executed only on render thread! */
      void SpinRenderThreadJobs();

   private:

      void ProcessPushRenderThreadJob(const EnqueueJobPolicy policy, const std::function<void(void)>& job);
      void ProcessPushGameThreadJob(const EnqueueJobPolicy policy, const std::function<void(void)>& job);

      inline bool AreGameJobsAwaiting() const {

         return m_gameThreadJobs.size() > 0;
      }

      inline bool AreRenderJobsAwaiting() const {

         return m_renderThreadJobs.size() > 0;
      }
   };

}

