#pragma once

#include "Actor.h"

namespace Game
{

   class PlayerController :
      public Actor
   {

      using Base = Actor;

      std::shared_ptr<Actor> m_playerActor;

   public:

      PlayerController();

      virtual ~PlayerController();
      
      void SetPlayerActor(std::shared_ptr<Actor> playerActor);

      virtual void Tick(float deltaTime) override;

      // invoke when player's position or orientation should be changed
      void UpdateTranslation();

      void UpdateRotation();
   };

}

