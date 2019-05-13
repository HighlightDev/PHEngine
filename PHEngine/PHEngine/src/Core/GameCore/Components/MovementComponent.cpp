#include "MovementComponent.h"

#include "Core/GameCore/CameraBase.h"

namespace Game
{

   MovementComponent::MovementComponent(CameraBase* const mainCamera, glm::vec3 launchVelocity)
      : Component()
      , m_mainCamera(mainCamera)
      , Velocity(launchVelocity)
      , Speed(0.005f)
   {
   }

   MovementComponent::~MovementComponent()
   {
   }

   CameraBase* const MovementComponent::GetCamera() const
   {
      return m_mainCamera;
   }

   uint64_t MovementComponent::GetComponentType() const
   {
      return MOVEMENT_COMPONENT;
   }
}
