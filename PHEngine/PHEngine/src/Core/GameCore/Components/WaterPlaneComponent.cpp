#include "WaterPlaneComponent.h"
#include "Core/GameCore/Scene.h"

#include <glm/vec3.hpp>

namespace Game
{

   WaterPlaneComponent::WaterPlaneComponent(WaterQualityFlag waterQuality)
      : PrimitiveComponent(glm::vec3(), glm::vec3(), glm::vec3())
      , m_renderData(nullptr, nullptr, nullptr)
      , m_waterQuality(waterQuality)
   {
      /*m_scene->ExecuteOnRenderThread([=]() {
        
         m_scene->SceneProxies[PrimitiveProxyComponentId]->waterdistortion = 0.01f;

      });*/
   }

   WaterPlaneComponent::~WaterPlaneComponent()
   {
   }
   
   ComponentType WaterPlaneComponent::GetComponentType() const
   {
      return WATER_PLANE_COMPONENT;
   }

}
