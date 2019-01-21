#include "Component.h"

#include <algorithm>

namespace Game
{
	namespace Components
	{

		Component::Component()
		{
		}

		Component::~Component()
		{

		}

		void Component::Tick(float deltaTime)
		{

		}

		void Component::AttachComponent(std::shared_ptr<Component> component)
		{
			m_children.push_back(component);
		}

		void Component::DetachComponent(std::shared_ptr<Component> component)
		{
			m_children.erase(std::find(m_children.begin(), m_children.end(), component));
		}

	}
}