#include "PointLightSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      PointLightSceneProxy::PointLightSceneProxy(const PointLightComponent* component)
         : LightSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().Ambient, component->GetRenderData().Diffuse, component->GetRenderData().Specular)
         , m_position(component->GetRenderData().Position)
         , m_attenuation(component->GetRenderData().Attenuation)
         , m_radianceSqrRadius(component->GetRenderData().RadianceSqrRadius)
      {

      }

      PointLightSceneProxy::~PointLightSceneProxy()
      {

      }

      LightSceneProxyType PointLightSceneProxy::GetLightProxyType() const {

         return LightSceneProxyType::POINT_LIGHT;
      }

   }
}
