#pragma once
#include "PoolBase.h"
#include "Core/GraphicsCore/Texture/ITexture.h"
#include "Core/GraphicsCore/Texture/TexParams.h"
#include "Core/ResourceManagerCore/Policy/RenderTargetAllocationPolicy.h"

using namespace Graphics::Texture;

namespace Resources
{
   class RenderTargetPool :
      public PoolBase<ITexture, TexParams, RenderTargetAllocationPolicy>
   {

      static std::unique_ptr<RenderTargetPool> m_instance;

   public:

      using poolType_t = PoolBase<ITexture, TexParams, RenderTargetAllocationPolicy>;

      RenderTargetPool();

      virtual ~RenderTargetPool();

      static std::unique_ptr<RenderTargetPool>& GetInstance() {

         if (!m_instance)
            m_instance = std::make_unique<RenderTargetPool>();

         return m_instance;
      }

      static void ReloadInstance()
      {
         if (m_instance)
            m_instance.reset();
      }

   };
}

