#include "SkeletalMeshSceneProxy.h"
#include "Core/GraphicsCore/Mesh/AnimatedSkin.h"

using namespace Graphics::Mesh;

namespace Graphics
{
   namespace Proxy
   {

      SkeletalMeshSceneProxy::SkeletalMeshSceneProxy(const SkeletalMeshComponent* component)
         : PrimitiveSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().m_skin,
            component->GetRenderData().m_albedo, component->GetRenderData().m_normalMapTex,
            component->GetRenderData().m_specularMapTex)
         , m_shader(std::dynamic_pointer_cast<SkeletalMeshShader>(component->GetRenderData().m_shader))
         , m_animations(component->GetRenderData().m_animations)
         , m_animationHolder(m_animations)
         , m_animationDeltaTime(component->GetAnimationDeltaTime())
      {
      }

      SkeletalMeshSceneProxy::~SkeletalMeshSceneProxy()
      {
      }

      void SkeletalMeshSceneProxy::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         AnimatedSkin* animatedSkin = static_cast<AnimatedSkin*>(m_skin.get());

         m_animationHolder.UpdateAnimationLoopTime(m_animationDeltaTime);
         std::vector<glm::mat4> skinningMatrices = m_animationHolder.GetAnimatedOffsetedMatrices(animatedSkin->GetRootBone().get());

         m_shader->ExecuteShader();
         m_albedoTex->BindTexture(0);
         m_shader->SetAlbedoTex(0);
         m_shader->SetTransformationMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);
         m_shader->SetSkinningMatrices(skinningMatrices);
         animatedSkin->GetBuffer()->RenderVAO(GL_TRIANGLES);
         m_shader->StopShader();
      }

      std::shared_ptr<ShaderBase> SkeletalMeshSceneProxy::GetShader()
      {
         return m_shader;
      }

      void SkeletalMeshSceneProxy::SetAnimationDeltaTime(float animationDeltaTime)
      {
         m_animationDeltaTime = animationDeltaTime;
      }

      uint64_t SkeletalMeshSceneProxy::GetComponentType() const
      {
         return SKELETAL_MESH_COMPONENT;
      }

      std::vector<glm::mat4> SkeletalMeshSceneProxy::GetSkinningMatrices()
      {
         AnimatedSkin* animatedSkin = static_cast<AnimatedSkin*>(m_skin.get());
         m_animationHolder.UpdateAnimationLoopTime(m_animationDeltaTime);
         return m_animationHolder.GetAnimatedOffsetedMatrices(animatedSkin->GetRootBone().get());
      }
   }
}
