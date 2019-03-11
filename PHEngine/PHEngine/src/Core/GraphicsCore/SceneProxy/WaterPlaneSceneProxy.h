#pragma once
#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/WaterPlaneComponent.h"
#include "Core/GameCore/FramebufferImplementation/WaterPlaneFramebuffer.h"

using namespace Game;
using namespace Game::FramebufferImpl;

namespace Graphics
{
   namespace Proxy
   {

      class WaterPlaneSceneProxy :
         public PrimitiveSceneProxy
      {

         std::unique_ptr<WaterPlaneFramebuffer> m_waterPlaneFramebuffer;
         std::shared_ptr<ITexture> m_waterDistortionMap;
         std::shared_ptr<ITexture> m_waterNormalMap;
         std::shared_ptr<Skin> m_skin;

      public:

         WaterPlaneSceneProxy(const WaterPlaneComponent* component);

         virtual ~WaterPlaneSceneProxy();

      private:

         void Init();
      };
   }
}

