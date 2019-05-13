#pragma once
#include "Component.h"

#include <glm/vec3.hpp>

namespace Game
{

   class MovementComponent :
      public Component
   {

      class CameraBase* const m_mainCamera;

   public:

      glm::vec3 Velocity;

      float Speed;

      MovementComponent(class  CameraBase* const mainCamera, glm::vec3 launchVelocity);

      virtual ~MovementComponent();

      virtual uint64_t GetComponentType() const override;

      class  CameraBase* const GetCamera() const;
   };

}
