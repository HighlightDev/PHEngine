#pragma once
#include "Component.h"
#include "Core/GameCore/Input/KeyboardBindings.h"

namespace Game
{

   class InputComponent :
      public Component
   {

      KeyboardBindings m_keyboardBindings;

   public:

      InputComponent();

      virtual ~InputComponent();

      virtual uint64_t GetComponentType() const override;

      // Game thread tick
      virtual void Tick(float deltaTime) override;

      KeyboardBindings& GetKeyboardBindings();
   };

}
