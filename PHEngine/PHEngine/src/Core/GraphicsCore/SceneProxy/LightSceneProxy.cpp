#include "LightSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      LightSceneProxy::LightSceneProxy(glm::mat4 relativeMatrix, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
         : m_relativeMatrix(relativeMatrix)
         , AmbientColor(ambientColor)
         , DiffuseColor(diffuseColor)
         , SpecularColor(specularColor)
      {
      }

      LightSceneProxy::~LightSceneProxy()
      {
      }

   }
}
