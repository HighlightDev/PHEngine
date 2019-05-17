#include "DirectionalLightSceneProxy.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Graphics
{
   namespace Proxy
   {

      DirectionalLightSceneProxy::DirectionalLightSceneProxy(const DirectionalLightComponent* component)
         : LightSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().Ambient, component->GetRenderData().Diffuse, component->GetRenderData().Specular, component->GetRenderData().ShadowInfo)
         , m_direction(component->GetRenderData().Direction)
      {
         if (this->m_shadowInfo)
         {
            // init
            m_shadowInfo->ShadowProjectionMatrices[0] = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f);
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
            // Build shadow view matrix
            glm::vec3 normLightDir = glm::normalize(GetDirection());
            
            // Target is the player, keep collecting shadow info when player is moving all around the level
            glm::vec3 targetPositon = m_shadowInfo->Offset;
            glm::vec3 lightTranslatedPosition = -(normLightDir * 50.0f);
            glm::vec3 shadowCastPosition(targetPositon + lightTranslatedPosition);

            m_shadowInfo->ShadowViewMatrices[0] = glm::lookAt(shadowCastPosition, targetPositon, glm::vec3(0, 1, 0));
         }

         return m_shadowInfo;
      }

   }
}
