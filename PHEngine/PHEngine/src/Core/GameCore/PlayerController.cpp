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
      Base::Tick(deltaTime);

   }

}