#pragma once
#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/StaticMeshComponent.h"
#include "Core/GameCore/Components/BillboardComponent.h"
#include "Core/GameCore/ShaderImplementation/BillboardShader.h"

using namespace Game;
using namespace Game::ShaderImpl;

namespace Graphics
{
   namespace Proxy
   {

      class BillboardSceneProxy :
         public PrimitiveSceneProxy
      {

         std::shared_ptr<BillboardShader> m_shader;

      protected:

         using Base = PrimitiveSceneProxy;

      public:

         BillboardSceneProxy(const BillboardComponent* component);

         ~BillboardSceneProxy();

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

         virtual std::shared_ptr<ShaderBase> GetShader() const override;
      };

   }
}

