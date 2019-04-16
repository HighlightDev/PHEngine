#include "DirectionalLightSceneProxy.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Graphics
{
   namespace Proxy
   {

      DirectionalLightSceneProxy::DirectionalLightSceneProxy(const DirectionalLightComponent* component)
         : LightSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().Ambient, component->GetRenderData().Diffuse, component->GetRenderData().Specular, component->ShadowInfo)
         , m_direction(component->GetRenderData().Direction)
      {

      }

      DirectionalLightSceneProxy::~DirectionalLightSceneProxy()
      {

      }

      LightSceneProxyType DirectionalLightSceneProxy::GetLightProxyType() const {

         return LightSceneProxyType::DIR_LIGHT;
      }

      const ProjectedShadowInfo* DirectionalLightSceneProxy::GetShadowInfo() const
      {
         if (this->m_shadowInfo)
         {
            // Build view and projection matrix
            m_shadowInfo->ShadowProjectionMatrices[0] = glm::ortho(0, 0, 0, 0, 0, 0);
            m_shadowInfo->ShadowViewMatrices[0] = glm::lookAt(glm::vec3(), glm::vec3(), glm::vec3());
         }

         return m_shadowInfo;
      }

   }
}
