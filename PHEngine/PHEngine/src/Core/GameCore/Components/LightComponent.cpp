#include "LightComponent.h"
#include "Core/GameCore/Scene.h"

namespace Game
{

   size_t LightComponent::TotalLightSceneProxyId = 0;

   LightComponent::LightComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
      : SceneComponent(translation, rotation, scale)
   {

   }

   LightComponent::~LightComponent()
   {

   }

   ComponentType LightComponent::GetComponentType() const
   {
      return LIGHT_COMPONENT;
   }

   void LightComponent::UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix)
   {
      Base::UpdateRelativeMatrix(parentRelativeMatrix);
      // Update light proxy transform
      m_scene->OnUpdateLightComponentTransform_GameThread(LightSceneProxyId, m_relativeMatrix);
   }

}