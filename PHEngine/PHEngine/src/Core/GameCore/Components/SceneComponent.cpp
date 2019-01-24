#include "SceneComponent.h"


namespace Game
{
	SceneComponent::SceneComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		: Component()
		, bTransformationDirty(false)
		, m_translation(translation)
		, m_rotation(rotation)
		, m_scale(scale)
	{
	}

	SceneComponent::~SceneComponent()
	{
	}

	void SceneComponent::Tick(float deltaTime)
	{
		Component::Tick(deltaTime);

		if (bTransformationDirty)
		{
			// Update transform matrix
			bTransformationDirty = false;
		}
	}

	void SceneComponent::UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix)
	{
		// Update current relative matrix
		// and whole hierarchy
	}

}
