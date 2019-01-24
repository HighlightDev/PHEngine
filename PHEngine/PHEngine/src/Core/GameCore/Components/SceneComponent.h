#pragma once
#include "Component.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Game
{
	class SceneComponent : public Component
	{
	private:

		bool bTransformationDirty;

		glm::vec3 m_translation;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
		// bound

		glm::mat4 m_relativeMatrix;

	public:

		SceneComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

		~SceneComponent();

		virtual void Tick(float deltaTime) override;

		virtual void UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix);

		void SetTranslation(glm::vec3 translation)
		{
			m_translation = translation;
			bTransformationDirty = true;
		}

		void SetRotation(glm::vec3 rotation)
		{
			m_rotation = rotation;
			bTransformationDirty = true;
		}

		void SetScale(glm::vec3 scale)
		{
			m_scale = scale;
			bTransformationDirty = true;
		}

		inline glm::vec3 GetTranslation()
		{
			return m_translation;
		}

		inline glm::vec3 GetRotation()
		{
			return m_rotation;
		}

		inline glm::vec3 GetScale()
		{
			return m_scale;
		}

		inline 	glm::mat4 GetRelativeMatrix()
		{
			return m_relativeMatrix;
		}
	};
}



