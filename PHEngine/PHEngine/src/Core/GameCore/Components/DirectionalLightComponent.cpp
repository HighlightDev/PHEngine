#include "DirectionalLightComponent.h"

#include "Core/GraphicsCore/SceneProxy/DirectionalLightSceneProxy.h"

namespace Game
{

   DirectionalLightComponent::DirectionalLightComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, DirectionalLightRenderData& renderData)
      : LightComponent(translation, rotation, scale)
      , m_renderData(renderData)
   {

   }

   DirectionalLightComponent::~DirectionalLightComponent()
   {

   }

   std::shared_ptr<LightSceneProxy> DirectionalLightComponent::CreateSceneProxy() const
   {
      return std::make_shared<DirectionalLightSceneProxy>(this);
   }

   void DirectionalLightComponent::UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix)
   {
      Base::UpdateRelativeMatrix(parentRelativeMatrix);

      SetRotationAxisY(m_rotation.y + 1);
   }

   ComponentType DirectionalLightComponent::GetComponentType() const
   {
      return DIR_LIGHT_COMPONENT;
   }

}
