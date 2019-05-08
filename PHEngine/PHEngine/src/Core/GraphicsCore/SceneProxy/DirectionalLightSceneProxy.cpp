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
            // init
            m_shadowInfo->ShadowProjectionMatrices[0] = glm::ortho(-200, 200, -200, 200, 1, 400);
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
         if (this->m_shadowInfo)
         {
            // Build view matrix

            glm::vec3 normLightDir = glm::normalize(m_direction);

            glm::vec3 targetPositon = m_shadowInfo->Offset;
            glm::vec3 lightTranslatedPosition = -(normLightDir * 300.0f);
            glm::vec3 shadowCastPosition(targetPositon + lightTranslatedPosition);

            m_shadowInfo->ShadowViewMatrices[0] = glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
               //glm::lookAt(shadowCastPosition, targetPositon, glm::vec3(0, 1, 0));
         }

         return m_shadowInfo;
      }

   }
}
