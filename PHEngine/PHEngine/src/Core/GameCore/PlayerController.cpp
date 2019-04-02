#include "PlayerController.h"
#include "Core/GameCore/CameraBase.h"

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

      auto& bindings = m_playerActor->GetInputComponent()->GetKeyboardBindings();
      std::shared_ptr<SceneComponent> rootComponent = m_playerActor->GetRootComponent();
      std::shared_ptr<MovementComponent> movementComponent = m_playerActor->GetMovementComponent();

      glm::vec3& velocity = movementComponent->Velocity;
      float& speed = movementComponent->Speed;
      const CameraBase* const camera = movementComponent->GetCamera();

      if (bindings.GetKeyState(Keys::W))
      {
         velocity = camera->GetEyeSpaceForwardVector() * glm::vec3(1, 0, 1); // truncate y-velocity

         auto newPosition = (velocity * speed) + rootComponent->GetTranslation();

         rootComponent->SetTranslation(newPosition);
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