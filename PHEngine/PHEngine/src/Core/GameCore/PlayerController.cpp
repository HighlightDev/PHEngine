#include "PlayerController.h"
#include "Core/GameCore/CameraBase.h"
#include "COre/GameCore/ThirdPersonCamera.h"
#include "Core/GameCore/Event/TEvent.h"

namespace Game
{

   PlayerController::PlayerController()
   {
   }

   PlayerController::~PlayerController()
   {
   }

   void PlayerController::SetPlayerActor(Actor* playerActor)
   {
      m_playerActor = playerActor;
   }

   void PlayerController::Tick(float deltaTime)
   {
      if (!m_playerActor)
         return; 

      if (m_playerActor->GetInputComponent())
      {
         auto& bindings = m_playerActor->GetInputComponent()->GetKeyboardBindings();
         std::shared_ptr<SceneComponent> rootComponent = m_playerActor->GetRootComponent();
         std::shared_ptr<MovementComponent> movementComponent = m_playerActor->GetMovementComponent();

         glm::vec3& velocity = movementComponent->Velocity;
         float& speed = movementComponent->Speed;
         CameraBase* const camera = movementComponent->GetCamera();

         if (bindings.GetKeyState(Keys::W))
         {
            velocity = camera->GetEyeSpaceForwardVector() * glm::vec3(1, 1, 1); // truncate y-velocity

            auto newPosition = (velocity * speed) + rootComponent->GetTranslation();

            rootComponent->SetTranslation(newPosition);

            if (camera->GetCameraType() == CameraBase::CameraType::THIRD_PERSON)
            {
               // TEST FAST VERSION
               auto myCamera = static_cast<ThirdPersonCamera*>(camera);
               if (myCamera->GetThirdPersonTarget() == m_playerActor)
               {
                  myCamera->SetThirdPersonTargetTransformationDirty();
               }
            }

            Event::CameraMovedEvent::GetInstance()->SendEvent(newPosition);
         }
         else if (bindings.GetKeyState(Keys::A))
         {
         }
         else if (bindings.GetKeyState(Keys::D))
         {
         }
         else if (bindings.GetKeyState(Keys::S))
         {
         }
      }

   }

}