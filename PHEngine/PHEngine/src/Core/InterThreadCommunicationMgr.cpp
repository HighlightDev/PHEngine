#include "InterThreadCommunicationMgr.h"

#include <chrono>
#include <iostream>

namespace Thread
{

   InterThreadCommunicationMgr::InterThreadCommunicationMgr()
   {


   }

   InterThreadCommunicationMgr::~InterThreadCommunicationMgr()
   {
   }

   void InterThreadCommunicationMgr::PushGameThreadJob(std::function<void(void)> job)
   {
      std::lock_guard<std::mutex> lock(m_gameThreadMutex);
      m_gameThreadJobs.push(job);
   }

   void InterThreadCommunicationMgr::PushRenderThreadJob(std::function<void(void)> job)
   {
      std::lock_guard<std::mutex> lock(m_renderThreadMutex);
      m_renderThreadJobs.push(job);
   }

   void InterThreadCommunicationMgr::SpinGameThreadJobs()
   {
      using Clock_t = std::chrono::high_resolution_clock;

      typename Clock_t::time_point start_time = Clock_t::now();

      std::lock_guard<std::mutex> lock(m_gameThreadMutex);
      while (AreGameJobsAwaiting())
      {
         auto job = m_gameThreadJobs.front();
         job();
         m_gameThreadJobs.pop();
         
         /* Measure how much time was spent on jobs execution
            If spent time is more than border - stop execution, 
            leave execution until next invocation        */
         typename Clock_t::time_point current_time = Clock_t::now();
         Clock_t::duration deltaTime = current_time - start_time;
         if (deltaTime.count() > 1e+7) // 0.01 seconds
            return;
      }
   }

   void InterThreadCommunicationMgr::SpinRenderThreadJobs()
   {
      using Clock_t = std::chrono::high_resolution_clock;

      typename Clock_t::time_point start_time = Clock_t::now();

      std::lock_guard<std::mutex> lock(m_renderThreadMutex);
      while (AreRenderJobsAwaiting())
      {
         auto job = m_renderThreadJobs.front();
         job();
         m_renderThreadJobs.pop();

         /* Measure how much time was spent on jobs execution
            If spent time is more than border - stop execution,
            leave execution until next invocation        */
         typename Clock_t::time_point current_time = Clock_t::now();
         Clock_t::duration deltaTime = current_time - start_time;

         std::cout << (deltaTime.count() / 1e+7) << std::endl;

         if (deltaTime.count() > 1e+7) // 0.01 seconds
            return;
      }
   }
}
