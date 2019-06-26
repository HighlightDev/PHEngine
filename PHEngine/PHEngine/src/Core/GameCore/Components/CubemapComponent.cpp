#include "CubemapComponent.h"
#include "Core/GraphicsCore/SceneProxy/CubemapSceneProxy.h"

namespace Game
{

   CubemapComponent::CubemapComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, const CubemapRenderData& renderData)
      : PrimitiveComponent(translation, rotation, scale)
      , m_renderData(renderData)
   {
   }

   CubemapComponent::~CubemapComponent()
   {
   }

   uint64_t CubemapComponent::GetComponentType() const
   {
      return PRIMITIVE_COMPONENT;
   }

   void CubemapComponent::Tick(float deltaTime)
   {

   }

   std::shared_ptr<PrimitiveSceneProxy> CubemapComponent::CreateSceneProxy() const
   {
      return std::make_shared<CubemapSceneProxy>(this);
   }
}