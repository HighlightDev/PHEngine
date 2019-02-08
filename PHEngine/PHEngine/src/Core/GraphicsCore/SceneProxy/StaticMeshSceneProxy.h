#pragma once
#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/StaticMeshComponent.h"
#include "Core/GameCore/ShaderImplementation/StaticMeshShader.h"

using namespace Game;
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
         StaticMeshSceneProxy(const StaticMeshComponent* component);

         ~StaticMeshSceneProxy();

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

         virtual std::shared_ptr<ShaderBase> GetShader() override;
      };

   }
}

