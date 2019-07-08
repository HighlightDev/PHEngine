#pragma once

#include "TEvent.h"

namespace Event
{

   class ComponentPositionChangedEvent
      : public TEvent<uint64_t>
   {
   public:
      using Event = TEvent<uint64_t>::Event_t;
         
   };

}