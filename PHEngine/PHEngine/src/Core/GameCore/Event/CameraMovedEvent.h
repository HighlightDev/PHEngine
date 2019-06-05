#pragma once

#include "TEvent.h"

namespace Event
{

   class CameraMovedEvent 
      : public TEvent<glm::vec3>
   {
   public:
      using Event = TEvent<glm::vec3>::Event_t;
         
   };

}