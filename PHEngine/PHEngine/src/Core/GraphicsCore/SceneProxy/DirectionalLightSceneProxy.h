#pragma once

#include "LightSceneProxy.h"
#include "Core/GameCore/Components/DirectionalLightComponent.h"

using namespace Game;

namespace Graphics
{
   namespace Proxy
   {

      class DirectionalLightSceneProxy :
         public LightSceneProxy
      {
         glm::vec3 m_direction;

      public:
         
         glm::vec3 GetDirection() const
         {
            glm::vec3 result = m_relativeMatrix * glm::vec4(m_direction, 0.0f);
            return result;
         }

         DirectionalLightSceneProxy(const DirectionalLightComponent* component);

         virtual ~DirectionalLightSceneProxy();

         virtual LightSceneProxyType GetLightProxyType() const override;
      };

   }
}

