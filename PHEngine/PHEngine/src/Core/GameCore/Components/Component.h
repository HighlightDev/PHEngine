#pragma once

#include "ComponentType.h"

#include <memory>
#include <mutex>
#include <vector>
#include <glm/vec3.hpp>

namespace Game
{
	namespace Components
	{
		// This is the base abstract class
		// of all components which could be 
		// picked by actor
		class Component
		{

			std::mutex m_lock;

		protected:

			ComponentType m_type;

			bool bTransformationDirty;

			glm::vec3 m_translation;
			glm::vec3 m_rotation;
			glm::vec3 m_scale;

			Component* m_parent;

			std::vector<std::shared_ptr<Component>> m_children;
			
			// bound

		public:

			Component();

			virtual ~Component();

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

			virtual ComponentType GetComponentType()
			{
				return ComponentType::UNDEFINED;
			}

			// Game thread tick
			virtual void Tick(float deltaTime);

			virtual void AttachComponent(std::shared_ptr<Component> component);

			virtual void DetachComponent(std::shared_ptr<Component> component);
		};

	}
}

