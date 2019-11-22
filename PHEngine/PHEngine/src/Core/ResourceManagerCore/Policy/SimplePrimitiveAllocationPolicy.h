#pragma once
#include <string>
#include <memory>
#include <stdint.h>

#include "Core/GraphicsCore/Mesh/Skin.h"

using namespace Graphics::Mesh;

namespace Resources
{
   enum class SimplePrimitiveType : int32_t
   {
      POINT,
      CUBE,
      PLANE
   };

   template <typename Model>
   class SimplePrimitiveAllocationPolicy
   {
   public:

      static std::shared_ptr<Skin> AllocateMemory(int32_t arg);

      static void DeallocateMemory(const std::shared_ptr<Skin>& arg);
   };

}

