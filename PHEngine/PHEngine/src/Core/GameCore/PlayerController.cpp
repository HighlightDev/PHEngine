#include "PlayerController.h"

namespace Game
{

   PlayerController::PlayerController()
      : Actor()
   {
   }


   PlayerController::~PlayerController()
   {
   }

   void PlayerController::SetPlayerActor(std::shared_ptr<Actor> playerActor)
   {

   }

   void PlayerController::Tick(float deltaTime)
   {
      auto& bindings = m_playerActor->GetInputComponent()->GetKeyboardBindings();

      std::shared_ptr<Actor> playerActor = m_playerActor;
      std::shared_ptr<Game::SceneComponent> rootComponent = playerActor->GetRootComponent();

      if (bindings.GetKeyState(Keys::W))
      {
         glm::vec3 velocity = glm::vec3(0, 0, 1);
         rootComponent->SetTranslation((velocity * 0.1f) + rootComponent->GetTranslation());
      }
      else if (bindings.GetKeyState(Keys::A))
      {
         glm::vec3 velocity = glm::vec3(-1, 0, 0);
         rootComponent->SetTranslation((velocity * 0.1f) + rootComponent->GetTranslation());
      }
      else if (bindings.GetKeyState(Keys::D))
      {
         glm::vec3 velocity = glm::vec3(1, 0, 1);
         rootComponent->SetTranslation((velocity * 0.1f) + rootComponent->GetTranslation());
      }
      else if (bindings.GetKeyState(Keys::S))
      {
         glm::vec3 velocity = glm::vec3(0, 0, -1);
         rootComponent->SetTranslation((velocity * 0.1f) + rootComponent->GetTranslation());
      }
          
      Base::Tick(deltaTime);

   }

}