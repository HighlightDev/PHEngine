#pragma once

#include <functional>
#include <stdint.h>

namespace Thread
{

   class Job
   {
      using callback_t = std::function<void(void)>;

      uint64_t mCreatorObjectId;
      callback_t mCallback;

   public:

      Job(const uint64_t creatorObjectId, const callback_t& callback);

      ~Job();

      uint64_t GetCreatorObjectId() const;
      callback_t GetCallback() const;

      void operator()() const;
   };

}

