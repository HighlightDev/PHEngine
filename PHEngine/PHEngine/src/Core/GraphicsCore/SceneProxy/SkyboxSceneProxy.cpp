#include "SkyboxSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      SkyboxSceneProxy::SkyboxSceneProxy(const SkyboxComponent* component)
         : PrimitiveSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().m_skin, std::shared_ptr<ITexture>(), std::shared_ptr<ITexture>())
         , m_skyboxShader(std::dynamic_pointer_cast<SkyboxShader>(component->GetRenderData().m_shader))
         , m_dayTexture(component->GetRenderData().m_dayTex)
         , m_nightTexture(component->GetRenderData().m_nightTex)
      {
         m_IsDeferred = false;
      }

      SkyboxSceneProxy::~SkyboxSceneProxy()
      {
      }

      void SkyboxSceneProxy::PostConstructor() 
      {
         if (bPostConstructor)
         {

            bPostConstructor = false;
         }
      }

      std::shared_ptr<ShaderBase> SkyboxSceneProxy::GetShader()
      {
         return m_skyboxShader;
      }

      void SkyboxSceneProxy::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         PostConstructor();

         glDisable(GL_CLIP_DISTANCE0);

         m_skyboxShader->ExecuteShader();

         if (m_dayTexture)
         {
            m_dayTexture->BindTexture(0);
         }
         if (m_nightTexture)
         {
            m_nightTexture->BindTexture(1);
         }
         m_skyboxShader->SetTransformMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);
         m_skyboxShader->SetTextures(0, 1);
         //CastToSkyboxShader()->SetDayCycleValue(sunDirection.Normalized().Y);
         //CastToSkyboxShader()->SetMist(m_mist);
         m_skin->GetBuffer()->RenderVAO(GL_TRIANGLES);
         m_skyboxShader->StopShader();
      }
   }
}