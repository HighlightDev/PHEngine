#pragma once

#include <functional>
#include <queue>
#include <mutex>

namespace Thread
{

#define ENQUEUE_RENDER_THREAD_JOB(InterThreadMgrInstance, JOB) \
InterThreadMgrInstance.PushRenderThreadJob(JOB)

#define ENQUEUE_GAME_THREAD_JOB(InterThreadMgrInstance, JOB) \
InterThreadMgrInstance.PushGameThreadJob(JOB)

#define SPIN_RENDER_THREAD_JOBS(InterThreadMgrInstance) \
InterThreadMgrInstance.SpinRenderThreadJobs();

#define SPIN_GAME_THREAD_JOBS(InterThreadMgrInstance) \
InterThreadMgrInstance.SpinGameThreadJobs();

   class InterThreadCommunicationMgr
   {

      std::mutex m_gameThreadMutex;

      std::mutex m_renderThreadMutex;

      std::queue<std::function<void(void)>> m_gameThreadJobs;

      std::queue<std::function<void(void)>> m_renderThreadJobs;

   public:

      InterThreadCommunicationMgr();

      ~InterThreadCommunicationMgr();

      void PushGameThreadJob(std::function<void(void)> job);

      void PushRenderThreadJob(std::function<void(void)> job);

      /* @ Should be executed only on game thread! */
      void SpinGameThreadJobs();

      /* @ Should be executed only on render thread! */
      void SpinRenderThreadJobs();

      inline bool AreGameJobsAwaiting() const {

         return m_gameThreadJobs.size() > 0;
      }

      inline bool AreRenderJobsAwaiting() const {

         return m_renderThreadJobs.size() > 0;
      }
   };

}

