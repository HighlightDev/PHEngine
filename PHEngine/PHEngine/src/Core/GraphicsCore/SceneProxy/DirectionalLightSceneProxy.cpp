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
         if (this->m_shadowInfo)
         {
            m_shadowInfo->ShadowProjectionMatrices.push_back(glm::ortho(-200, 200, -200, 200, 1, 400));
            m_shadowInfo->ShadowViewMatrices.push_back(glm::mat4(1));
         }
      }

      DirectionalLightSceneProxy::~DirectionalLightSceneProxy()
      {

      }

      LightSceneProxyType DirectionalLightSceneProxy::GetLightProxyType() const {

         return LightSceneProxyType::DIR_LIGHT;
      }

      ProjectedShadowInfo* DirectionalLightSceneProxy::GetShadowInfo() const
      {
         //if (this->m_shadowInfo)
         //{
         //   // Build view matrix
         //   m_shadowInfo->ShadowViewMatrices[0] = glm::lookAt(glm::vec3(), glm::vec3(), glm::vec3());
         //}

         return m_shadowInfo;
      }

   }
}
