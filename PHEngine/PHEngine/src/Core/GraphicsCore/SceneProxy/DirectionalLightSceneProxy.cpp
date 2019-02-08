#include "DirectionalLightSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      DirectionalLightSceneProxy::DirectionalLightSceneProxy(const DirectionalLightComponent* component)
         : LightSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().Ambient, component->GetRenderData().Diffuse, component->GetRenderData().Specular)
         , m_direction(component->GetRenderData().Direction)
      {

      }

      DirectionalLightSceneProxy::~DirectionalLightSceneProxy()
      {

      }

      LightSceneProxyType DirectionalLightSceneProxy::GetLightProxyType() const {

         return LightSceneProxyType::DIR_LIGHT;
      }

   }
}
