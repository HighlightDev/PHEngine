#include "PrimitiveComponent.h"

namespace Game
{

	PrimitiveComponent::PrimitiveComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		: SceneComponent(std::move(translation), std::move(rotation), std::move(scale))
	{
	}

	PrimitiveComponent::~PrimitiveComponent()
	{
	}
}