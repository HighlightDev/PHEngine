#include "PointLightComponent.h"
#include "Core/GraphicsCore/SceneProxy/PointLightSceneProxy.h"
#include "Core/UtilityCore/EngineMath.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Core/GameCore/Actor.h"

namespace Game
{

   PointLightComponent::PointLightComponent(glm::vec3 translation, glm::vec3 rotation, const PointLightRenderData& renderData)
      : LightComponent(translation, rotation, glm::vec3(1))
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

   uint64_t PointLightComponent::GetComponentType() const
   {
      return POINT_LIGHT_COMPONENT;
   }

   float time = 0.0f;

   void PointLightComponent::Tick(float deltaTime)
   {
      Base::Tick(deltaTime);

      time += deltaTime;

      if (time > 360.0f)
      {
         time -= 360.0f;
      }
      
      auto translation = GetTranslation();

      //SetTranslation(glm::vec3(translation.x + std::cos(time) * 5.0f, translation.y + std::sin(time) * 5.0f, 0.0f));
   }

}