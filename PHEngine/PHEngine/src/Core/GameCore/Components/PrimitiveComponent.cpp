#include "PrimitiveComponent.h"

#include "Core/GameCore/Scene.h"

namespace Game
{

	PrimitiveComponent::PrimitiveComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		: SceneComponent(std::move(translation), std::move(rotation), std::move(scale))
	{
	}

	PrimitiveComponent::~PrimitiveComponent()
	{
	}

   void PrimitiveComponent::UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix)
   {
      Base::UpdateRelativeMatrix(parentRelativeMatrix);

      // Update primitives proxy transform
      m_scene->OnUpdatePrimitiveTransform_GameThread(SceneProxyComponentId, m_relativeMatrix);
   }
}