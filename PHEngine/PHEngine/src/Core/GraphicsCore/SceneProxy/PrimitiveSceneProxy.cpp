#include "PrimitiveSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      PrimitiveSceneProxy::PrimitiveSceneProxy(glm::mat4 relativeMatrix, std::shared_ptr<Skin> skin, std::shared_ptr<ITexture> albedoTex, std::shared_ptr<ITexture> normalMapTex,
         std::shared_ptr<ITexture> specularMapTex)
         : m_relativeMatrix(relativeMatrix)
         , m_skin(skin)
         , m_albedoTex(albedoTex)
         , m_normalMapTex(normalMapTex)
         , m_specularMapTex(specularMapTex)
      {
      }

      PrimitiveSceneProxy::~PrimitiveSceneProxy()
      {
      }

      void PrimitiveSceneProxy::PostConstructorInitialize()
      {
      }

      glm::mat4 PrimitiveSceneProxy::GetMatrix() const
      {
         return m_relativeMatrix;
      }

      void PrimitiveSceneProxy::SetTransformationMatrix(const glm::mat4& relativeMatrix)
      {
         m_relativeMatrix = relativeMatrix;
      }

      std::shared_ptr<Skin> PrimitiveSceneProxy::GetSkin() const
      {
         return m_skin;
      }

      std::shared_ptr<ITexture> PrimitiveSceneProxy::GetAlbedo() const
      {
         return m_albedoTex;
      }

      std::shared_ptr<ITexture> PrimitiveSceneProxy::GetNormalMap() const
      {
         return m_normalMapTex;
      }

      std::shared_ptr<ITexture> PrimitiveSceneProxy::GetSpecularMap() const
      {
         return m_specularMapTex;
      }

      uint64_t PrimitiveSceneProxy::GetComponentType() const
      {
         return PRIMITIVE_COMPONENT;
      }

   }
}