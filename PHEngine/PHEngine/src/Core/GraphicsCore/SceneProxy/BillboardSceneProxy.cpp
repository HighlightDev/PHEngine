#include "BillboardSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      BillboardSceneProxy::BillboardSceneProxy(const BillboardComponent* component)
         : PrimitiveSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().m_skin, component->GetRenderData().m_texture)
         , m_shader(std::static_pointer_cast<BillboardShader>(component->GetRenderData().m_shader))
      {
         m_IsDeferred = false;
      }

      BillboardSceneProxy::~BillboardSceneProxy()
      {
      }

      std::shared_ptr<ShaderBase> BillboardSceneProxy::GetShader() const
      {
         return m_shader;
      }

      void BillboardSceneProxy::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         auto billboardShader = std::static_pointer_cast<BillboardShader>(m_shader);

         billboardShader->ExecuteShader();

         m_albedoTex->BindTexture(0);

         billboardShader->SetTexture(0);
         billboardShader->SetTransformMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);

         m_skin->GetBuffer()->RenderVAO(GL_POINTS);
         billboardShader->StopShader();
      }

   }
}
