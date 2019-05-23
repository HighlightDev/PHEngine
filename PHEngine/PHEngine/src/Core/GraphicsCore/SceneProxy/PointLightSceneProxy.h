#pragma once

#include "LightSceneProxy.h"
#include "Core/GameCore/Components/PointLightComponent.h"

using namespace Game;

namespace Graphics
{
   namespace Proxy
   {

      class PointLightSceneProxy :
         public LightSceneProxy
      {
         glm::vec3 m_attenuation;
         float m_radianceSqrRadius;

      public:
         
         glm::vec3 GetPosition() const
         {
            glm::vec3 result = m_relativeMatrix * glm::vec4(0, 0, 0, 1.0f);
            return result;
         }

         glm::vec3 GetAttenuation() const {
            return m_attenuation;
         }

         float GetRadianceSqrRadius() const {
            return m_radianceSqrRadius;
         }

         PointLightSceneProxy(const PointLightComponent* component);

         virtual ~PointLightSceneProxy();

         virtual LightSceneProxyType GetLightProxyType() const override;
      };

   }
}

