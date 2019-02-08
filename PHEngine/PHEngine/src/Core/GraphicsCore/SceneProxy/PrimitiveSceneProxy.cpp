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
   }
}