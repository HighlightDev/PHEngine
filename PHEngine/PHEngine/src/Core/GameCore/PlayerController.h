#pragma once

#include "Actor.h"

namespace Game
{

   class PlayerController
   {

      using Base = Actor;

      Actor* m_playerActor;

   public:

      PlayerController();

      virtual ~PlayerController();
      
      void SetPlayerActor(Actor* playerActor);

      void Tick(float deltaTime);
   };

}

