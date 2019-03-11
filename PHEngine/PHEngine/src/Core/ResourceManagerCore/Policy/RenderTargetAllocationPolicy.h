#pragma once

#include <memory>
#include "Core/GraphicsCore/Texture/TexParams.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

using namespace Graphics::Texture;

namespace Resources
{

   class RenderTargetAllocationPolicy
   {
   public:
      RenderTargetAllocationPolicy();
      ~RenderTargetAllocationPolicy();

      static std::shared_ptr<ITexture> AllocateMemory(TexParams& texParams);

      static void DeallocateMemory(std::shared_ptr<ITexture> arg);
   };

}
