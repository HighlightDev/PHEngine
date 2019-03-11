#include "RenderTargetAllocationPolicy.h"
#include "Core/GraphicsCore/Texture/Texture2d.h"

namespace Resources
{

   RenderTargetAllocationPolicy::RenderTargetAllocationPolicy()
   {
   }


   RenderTargetAllocationPolicy::~RenderTargetAllocationPolicy()
   {
   }

   std::shared_ptr<ITexture> RenderTargetAllocationPolicy::AllocateMemory(TexParams& texParams)
   {
      return std::make_shared<Texture2d>(texParams);
   }

   void RenderTargetAllocationPolicy::DeallocateMemory(std::shared_ptr<ITexture> arg)
   {
      arg->CleanUp();
   }
}