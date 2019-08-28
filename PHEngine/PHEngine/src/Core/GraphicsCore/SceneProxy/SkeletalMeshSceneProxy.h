#pragma once

#include <vector>

#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/SkeletalMeshComponent.h"
#include "Core/GameCore/ShaderImplementation/SkeletalMeshShader.h"
#include "Core/GraphicsCore/Animation/AnimationSequence.h"
#include "Core/GraphicsCore/Animation/AnimationHolder.h"
#include "Core/GraphicsCore/OpenGL/Shader/CompositeShader.h"
#include "Core/GameCore/ShaderImplementation/VertexFactoryImp/SkeletalMeshVertexFactory.h"
#include "Core/GameCore/ShaderImplementation/DeferredCollectShader.h"

using namespace Game;
using namespace Game::ShaderImpl;
using namespace Graphics::Animation;

namespace Graphics
{
   namespace Proxy
   {

      class SkeletalMeshSceneProxy :
         public PrimitiveSceneProxy
      {
         using VertexFactoryType = SkeletalMeshVertexFactory<3>;
         using CompositeShaderType = DeferredCollectShader;
         using Base = PrimitiveSceneProxy;

         std::shared_ptr<SkeletalMeshShader> m_shader;
         std::shared_ptr<std::vector<AnimationSequence>> m_animations;

         AnimationHolder m_animationHolder;

         float m_animationDeltaTime;

         bool bAnimationTransformationDirty = true;

         std::shared_ptr<ICompositeShader> m_shaderTEMPMAIN;
         std::shared_ptr<IMaterial> m_material;

      public:
         SkeletalMeshSceneProxy(const SkeletalMeshComponent* component);

         virtual ~SkeletalMeshSceneProxy();

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

         virtual std::shared_ptr<ShaderBase> GetShader() const override;

         void SetAnimationDeltaTime(float animationDeltaTime);

         virtual uint64_t GetComponentType() const override;

         std::vector<glm::mat4> GetSkinningMatrices();
      };

   }
}
