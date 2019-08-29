#include "SkeletalMeshSceneProxy.h"
#include "Core/GraphicsCore/Mesh/AnimatedSkin.h"

using namespace Graphics::Mesh;

namespace Graphics
{
   namespace Proxy
   {

      SkeletalMeshSceneProxy::SkeletalMeshSceneProxy(const SkeletalMeshComponent* component)
         : PrimitiveSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().m_skin,
           nullptr, nullptr, nullptr)

         , m_shader(std::static_pointer_cast<ShaderType>(component->GetRenderData().m_materialShader))

         , m_animations(component->GetRenderData().m_animations)
         , m_animationHolder(m_animations)
         , m_animationDeltaTime(component->GetAnimationDeltaTime())
      {
         m_IsDeferred = true;
         m_animationHolder.SetAnimationByNameNoBlend(m_animations->at(0).GetName());
      }

      SkeletalMeshSceneProxy::~SkeletalMeshSceneProxy()
      {
      }

      void SkeletalMeshSceneProxy::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         AnimatedSkin* animatedSkin = static_cast<AnimatedSkin*>(m_skin.get());

         if (bAnimationTransformationDirty)
         {
            m_animationHolder.UpdateAnimationLoopTime(m_animationDeltaTime);
            bAnimationTransformationDirty = false;
         }

         std::vector<glm::mat4> skinningMatrices = m_animationHolder.GetAnimatedOffsetedMatrices(animatedSkin->GetRootBone().get());

         m_shader->ExecuteShader();
         m_shader->GetVertexFactoryShader()->SetMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);
         m_shader->GetVertexFactoryShader()->SetSkinningMatrices(skinningMatrices);
         m_shader->GetMaterialShader()->SetUniformValues();
         animatedSkin->GetBuffer()->RenderVAO(GL_TRIANGLES);
         m_shader->StopShader();
      }

      std::shared_ptr<ShaderBase> SkeletalMeshSceneProxy::GetShader() const
      {
         return nullptr;
      }

      void SkeletalMeshSceneProxy::SetAnimationDeltaTime(float animationDeltaTime)
      {
         m_animationDeltaTime = animationDeltaTime;
         bAnimationTransformationDirty = true;
      }

      uint64_t SkeletalMeshSceneProxy::GetComponentType() const
      {
         return SKELETAL_MESH_COMPONENT;
      }

      std::vector<glm::mat4> SkeletalMeshSceneProxy::GetSkinningMatrices()
      {
         AnimatedSkin* animatedSkin = static_cast<AnimatedSkin*>(m_skin.get());

         if (bAnimationTransformationDirty)
         {
            m_animationHolder.UpdateAnimationLoopTime(m_animationDeltaTime);
            bAnimationTransformationDirty = false;
         }

         return m_animationHolder.GetAnimatedOffsetedMatrices(animatedSkin->GetRootBone().get());
      }
   }
}
