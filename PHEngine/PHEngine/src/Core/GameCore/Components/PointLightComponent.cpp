#include "PointLightComponent.h"
#include "Core/GraphicsCore/SceneProxy/PointLightSceneProxy.h"

namespace Game
{

   PointLightComponent::PointLightComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, PointLightRenderData& renderData)
      : LightComponent(translation, rotation, scale)
      , m_renderData(renderData)
   {

   }

   PointLightComponent::~PointLightComponent()
   {

   }

   std::shared_ptr<LightSceneProxy> PointLightComponent::CreateSceneProxy() const
   {
      return std::make_shared<PointLightSceneProxy>(this);
   }

   ComponentType PointLightComponent::GetComponentType() const
   {
      return POINT_LIGHT_COMPONENT;
   }

}