#include "RenderTargetPool.h"

namespace Resources
{

   std::unique_ptr<RenderTargetPool> RenderTargetPool::m_instance;

   RenderTargetPool::RenderTargetPool()
      : PoolBase<ITexture, TexParams, RenderTargetAllocationPolicy>()
   {
   }


   RenderTargetPool::~RenderTargetPool()
   {
   }
}
