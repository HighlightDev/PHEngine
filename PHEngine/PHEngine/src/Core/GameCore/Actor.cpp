#include "Actor.h"
#include "Core/GameCore/Components/SceneComponent.h"
#include "Core/GameCore/Components/ComponentType.h"

using namespace Game::Components;

namespace Game
{

	Actor::Actor()
	{
	}


	Actor::~Actor()
	{
	}

	void Actor::Tick(float deltaTime)
	{

	}

	// Render is executed on render thread
	void Actor::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, float deltaTime)
	{
		for (auto it = m_allComponents.begin(); it != m_allComponents.end(); ++it)
		{
			if ((*it)->GetComponentType() == ComponentType::SCENE_COMPONENT)
			{
				std::shared_ptr<SceneComponent> sceneComponent = std::dynamic_pointer_cast<SceneComponent>(*it);
				sceneComponent->Render(viewMatrix, projectionMatrix);
			}
		}
	}

	void Actor::AddComponent(std::shared_ptr<Component> component)
	{
		m_allComponents.push_back(component);
	}

	void Actor::RemoveComponent(std::shared_ptr<Component> component)
	{
		m_allComponents.erase(std::find(m_allComponents.begin(), m_allComponents.end(), component));
	}

}
