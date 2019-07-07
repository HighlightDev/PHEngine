#include "PrimitiveComponent.h"

#include "Core/GameCore/Scene.h"

namespace Game
{

   size_t PrimitiveComponent::TotalPrimitiveSceneProxyIndex = 0;

	PrimitiveComponent::PrimitiveComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		: SceneComponent(std::move(translation), std::move(rotation), std::move(scale))
	{
	}

	PrimitiveComponent::~PrimitiveComponent()
	{
	}

   uint64_t PrimitiveComponent::GetComponentType() const
   {
      return PRIMITIVE_COMPONENT;
   }

   void PrimitiveComponent::UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix)
   {
      Base::UpdateRelativeMatrix(parentRelativeMatrix);

      // Update primitives proxy transform
      m_scene->OnUpdatePrimitiveComponentTransform_GameThread(PrimitiveProxyComponentId, GetObjectId(), m_relativeMatrix);
   }
}