#include "CubemapSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      CubemapSceneProxy::CubemapSceneProxy(const CubemapComponent* component)
         : PrimitiveSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().m_skin, std::shared_ptr<ITexture>(nullptr))
         , m_shader(std::static_pointer_cast<CubemapShader>(component->GetRenderData().m_shader))
         , m_textureObtainer(component->GetRenderData().m_texture)
      {
         m_IsDeferred = false;
      }

      CubemapSceneProxy::~CubemapSceneProxy()
      {
      }

      std::shared_ptr<ShaderBase> CubemapSceneProxy::GetShader() const
      {
         return m_shader;
      }

      void CubemapSceneProxy::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         std::shared_ptr<TextureAtlasHandler> texHandler = m_textureObtainer.GetTextureAtlasCellResource();
         if (texHandler && texHandler->GetTextureType() == TextureType::TEXTURE_CUBE)
         {
            std::shared_ptr<ITexture> texture = texHandler->GetAtlasResource();

            auto cubemapShader = std::static_pointer_cast<CubemapShader>(m_shader);

            cubemapShader->ExecuteShader();
            texture->BindTexture(0);
            cubemapShader->SetTexture(0);
            cubemapShader->SetTransformMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);
            m_skin->GetBuffer()->RenderVAO(GL_TRIANGLES);
            cubemapShader->StopShader();
         }
      }

   }
}
