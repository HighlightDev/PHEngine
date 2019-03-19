#include "InputComponent.h"
#include "Core/GameCore/KeyboardInputManager.h"

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

   KeyboardBindings& InputComponent::GetKeyboardBindings()
   {
      return m_keyboardBindings;
   }

   // Game thread tick
   void InputComponent::Tick(float deltaTime)
   {
      if (KeyboardInputManager::GetInstance()->HasKeyEvents())
      {
         KeyboardInputManager::queue_t& keyEventsQueue = KeyboardInputManager::GetInstance()->GetKeyboardKeyEvents();

         while (keyEventsQueue.size() > 0)
         {
            std::pair<Keys, size_t>& pair = keyEventsQueue.front();
            keyEventsQueue.pop();
            if (pair.second == KEY_PRESSED)
            {
               m_keyboardBindings.KeyPress(pair.first);
            }
            else
            {
               m_keyboardBindings.KeyRelease(pair.first);
            }
         }
      }
   }

}