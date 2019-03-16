#include "KeyboardBindings.h"

#include <algorithm>

namespace Game
{

   KeyboardBindings::KeyboardBindings()
   {
   }


   KeyboardBindings::~KeyboardBindings()
   {
   }

   bool KeyboardBindings::HasPressedKeys() const
   {
      auto pressedItem = std::find_if(keyboardMaskMap.begin(), keyboardMaskMap.end(), [](const std::pair<Keys, bool>& pair) { return pair.second; });
      return pressedItem != keyboardMaskMap.end();
   }

   void KeyboardBindings::AllocateKey(Keys key)
   {
      keyboardMaskMap.emplace(std::make_pair(key, false));
   }

   void  KeyboardBindings::KeyPress(Keys key)
   {
      auto it = keyboardMaskMap.find(key);
      if (it != keyboardMaskMap.end())
         it->second = true;
   }

   void KeyboardBindings::KeyRelease(Keys key)
   {
      auto it = keyboardMaskMap.find(key);
      if (it != keyboardMaskMap.end())
         it->second = false;
   }

   bool KeyboardBindings::GetKeyState(Keys key)
   {
      bool bEnabled = false;

      auto it = keyboardMaskMap.find(key);
      if (it != keyboardMaskMap.end())
         bEnabled = it->second;

      return bEnabled;
   }

}