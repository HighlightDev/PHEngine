#include "WaterPlaneComponent.h"

#include <glm/vec3.hpp>

namespace Game
{

   WaterPlaneComponent::WaterPlaneComponent()
      : PrimitiveComponent(glm::vec3(), glm::vec3(), glm::vec3())
      , m_renderData(nullptr, nullptr, nullptr)
   {
   }


   WaterPlaneComponent::~WaterPlaneComponent()
   {
   }

}
