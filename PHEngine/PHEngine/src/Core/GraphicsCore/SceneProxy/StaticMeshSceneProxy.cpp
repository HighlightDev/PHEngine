#include "StaticMeshSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      StaticMeshSceneProxy::StaticMeshSceneProxy(const StaticMeshComponent* component)
         : PrimitiveSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().m_skin, component->GetRenderData().m_albedo, component->GetRenderData().m_normalMapTex, component->GetRenderData().m_specularMapTex)
         , m_shader(std::static_pointer_cast<StaticMeshShader>(component->GetRenderData().m_shader))
      {
         m_IsDeferred = true;
      }

      StaticMeshSceneProxy::~StaticMeshSceneProxy()
      {
      }

      std::shared_ptr<ShaderBase> StaticMeshSceneProxy::GetShader()
      {
         return m_shader;
      }

      void StaticMeshSceneProxy::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         /*if (clipPlane.X == 0 && clipPlane.Y == 0 && clipPlane.Z == 0 && clipPlane.W == 0) { GL.Disable(EnableCap.ClipDistance0); }
         else { GL.Enable(EnableCap.ClipDistance0); }*/

         m_shader->ExecuteShader();

         if (m_albedoTex) m_albedoTex->BindTexture(0);
         if (m_normalMapTex) m_normalMapTex->BindTexture(1);
         if (m_specularMapTex) m_specularMapTex->BindTexture(2);

         m_shader->SetAlbedoTex(0);
         m_shader->SetNormalMapTex(1);
         m_shader->SetSpecularMapTex(2);
         //m_shader->SetMaterial(m_material);
         m_shader->SetTransformationMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);
         //m_shader->SetPointLights(GetRelevantPointLights(lights));
         //m_shader->SetDirectionalLight(directionalLight);
         //m_shader->SetClippingPlane(ref clipPlane);
         //m_shader->SetMist(m_mist);

         //if (directionalLight != null && directionalLight.GetHasShadow())
         //{
         //	DirectionalLightWithShadow lightWithShadow = directionalLight as DirectionalLightWithShadow;
         //	ITexture shadowMap = lightWithShadow.GetShadowMapTexture();
         //	shadowMap.BindTexture(TextureUnit.Texture4); // shadowmap
         //	GetShader().SetDirectionalLightShadowMatrix(lightWithShadow.GetShadowMatrix(ref modelMatrix, ref ProjectionMatrix));
         //}
         //GetShader().SetDirectionalLightShadowMap(4);

         if (m_skin) m_skin->GetBuffer()->RenderVAO(GL_TRIANGLES);
         m_shader->StopShader();
      }

   }
}
