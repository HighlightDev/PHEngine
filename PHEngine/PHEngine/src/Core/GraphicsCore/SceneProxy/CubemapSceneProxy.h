#pragma once
#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/CubemapComponent.h"
#include "Core/GameCore/ShaderImplementation/CubemapShader.h"

using namespace Game;
using namespace Game::ShaderImpl;

namespace Graphics
{
   namespace Proxy
   {

      class CubemapSceneProxy :
         public PrimitiveSceneProxy
      {

         std::shared_ptr<CubemapShader> m_shader;
         LazyTextureAtlasObtainer m_textureObtainer;

      protected:

         using Base = PrimitiveSceneProxy;

      public:

         CubemapSceneProxy(const CubemapComponent* component);

         ~CubemapSceneProxy();

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

         virtual std::shared_ptr<ShaderBase> GetShader() const override;
      };

   }
}

