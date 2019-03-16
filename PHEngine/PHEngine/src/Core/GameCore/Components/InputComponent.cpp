#include "InputComponent.h"

namespace Game
{

   InputComponent::InputComponent()
   {
   }


   InputComponent::~InputComponent()
   {
   }

   uint64_t InputComponent::GetComponentType() const
   {
      return INPUT_COMPONENT;
   }

   // Game thread tick
   void InputComponent::Tick(float deltaTime)
   {
      if (m_keyboardBindings.HasPressedKeys())
      {

      }
   }

}