#pragma once
#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/ShaderImplementation/StaticMeshShader.h"

using namespace Game::ShaderImpl;

namespace Graphics
{
   namespace Proxy
   {

      class StaticMeshSceneProxy :
         public PrimitiveSceneProxy
      {

         std::shared_ptr<StaticMeshShader> m_shader;

      protected:

         using Base = PrimitiveSceneProxy;

      public:
         StaticMeshSceneProxy(glm::mat4& relativeMatrix, std::shared_ptr<Skin> skin, std::shared_ptr<ShaderBase> shader, std::shared_ptr<ITexture> albedoTex, std::shared_ptr<ITexture> normalMapTex,
            std::shared_ptr<ITexture> specularMapTex);

         ~StaticMeshSceneProxy();

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

         virtual std::shared_ptr<ShaderBase> GetShader() override;
      };

   }
}

