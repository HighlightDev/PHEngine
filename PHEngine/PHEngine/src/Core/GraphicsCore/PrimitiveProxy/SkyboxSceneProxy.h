#pragma once

#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/ShaderImplementation/SkyboxShader.h"

using namespace Game::ShaderImpl;

namespace Graphics
{
   namespace Proxy
   {

      class SkyboxSceneProxy :
         public PrimitiveSceneProxy
      {
         bool bPostConstructor = true;

         std::shared_ptr<SkyboxShader> m_skyboxShader;
         std::shared_ptr<ITexture> m_dayTexture;
         std::shared_ptr<ITexture> m_nightTexture;

      protected:

         using Base = PrimitiveSceneProxy;

      public:

         SkyboxSceneProxy(glm::mat4& relativeMatrix, std::shared_ptr<Skin> skyboxMesh,
            std::shared_ptr<ShaderBase> skyboxShader, std::shared_ptr<ITexture> dayTexture, std::shared_ptr<ITexture> nightTexture);

         ~SkyboxSceneProxy();

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

         virtual std::shared_ptr<ShaderBase> GetShader() override;

         void PostConstructor();
      };

   }
}

