#include "SceneComponent.h"
#include "Core/UtilityCore/EngineMath.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace EngineUtility;

namespace Game
{

   SceneComponent::SceneComponent()
      : Component()
      , bTransformationDirty(true)
      , m_translation(0)
      , m_rotation(0)
      , m_scale(1)
      , m_relativeMatrix(1)
   {

   }

	SceneComponent::SceneComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		: Component()
		, bTransformationDirty(true)
		, m_translation(translation)
		, m_rotation(rotation)
		, m_scale(scale)
		, m_relativeMatrix(std::move(glm::mat4(1)))
	{
	}

	SceneComponent::~SceneComponent()
	{
	}

	void SceneComponent::Tick(float deltaTime)
	{
		Base::Tick(deltaTime);
	
	}

	void SceneComponent::UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix)
	{
		// Update current relative matrix

		const float pitchRad = DEG_TO_RAD(m_rotation.x);
		const float yawRad = DEG_TO_RAD(m_rotation.y);
		const float rollRad = DEG_TO_RAD(m_rotation.z);

		glm::mat4 worldMatrix(1);
		worldMatrix = glm::scale(worldMatrix, m_scale);
		worldMatrix = glm::rotate(worldMatrix, pitchRad, AXIS_RIGHT);
		worldMatrix = glm::rotate(worldMatrix, yawRad, AXIS_UP);
		worldMatrix = glm::rotate(worldMatrix, rollRad, AXIS_FORWARD);
		worldMatrix = glm::translate(worldMatrix, m_translation);
		worldMatrix = parentRelativeMatrix * worldMatrix;

		m_relativeMatrix = std::move(worldMatrix);

		bTransformationDirty = false;
	}

}
