#include "Job.h"

namespace Thread
{

      Job::Job(const uint64_t creatorObjectId, const callback_t& callback)
         : mCreatorObjectId(creatorObjectId)
         , mCallback(callback)
      {
      }

      Job::~Job()
      {
      }

      uint64_t Job::GetCreatorObjectId() const
      {
         return mCreatorObjectId;
      }

      Job::callback_t Job::GetCallback() const
      {
         return mCallback;
      }

      void Job::operator()() const
      {
         mCallback();
      }
}
