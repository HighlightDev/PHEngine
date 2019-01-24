#include "Actor.h"
#include "Core/GameCore/Components/PrimitiveComponent.h"
#include "Core/GameCore/Components/ComponentType.h"

namespace Game
{

	Actor::Actor(Game::SceneComponent* rootComponent)
	{
		m_rootComponent = std::shared_ptr<Game::SceneComponent>(rootComponent);
	}

	Actor::~Actor()
	{
	}

	void Actor::Tick(float deltaTime)
	{
		// Tick root component and if transform was dirty -> update all hierarchy of components

		if (m_rootComponent)
		{
			m_rootComponent->Tick(deltaTime);

			glm::mat4 parentRelativeMatrix(1);	// identity matrix

			if (m_parent)
				parentRelativeMatrix = m_parent->GetRootComponent()->GetRelativeMatrix();

			m_rootComponent->UpdateRelativeMatrix(parentRelativeMatrix);
		}

		for (auto it = m_allComponents.begin(); it != m_allComponents.end(); ++it)
		{
			// update all children components
			(*it)->Tick(deltaTime);
		}

		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			// update all attached actors
			(*it)->Tick(deltaTime);
		}
	}

	// Render is executed on render thread
	void Actor::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, float deltaTime)
	{
		for (auto it = m_allComponents.begin(); it != m_allComponents.end(); ++it)
		{
			if ((*it)->GetComponentType() == Game::ComponentType::PRIMITIVE_COMPONENT)
			{
				Game::PrimitiveComponent* primitiveComponent = static_cast<Game::PrimitiveComponent*>((*it).get());
				primitiveComponent->Render(viewMatrix, projectionMatrix);
			}
		}
	}

	void Actor::AddComponent(std::shared_ptr<Game::Component> component)
	{
		component->SetOwner(this);
		m_allComponents.push_back(component);
	}

	void Actor::RemoveComponent(std::shared_ptr<Game::Component> component)
	{
		auto componentIt = std::find(m_allComponents.begin(), m_allComponents.end(), component);
		if (componentIt != m_allComponents.end())
		{
			component->SetOwner(nullptr);
			m_allComponents.erase(componentIt);
		}
	}

	void Actor::SetParent(std::shared_ptr<Actor> actor)
	{
		m_parent = actor;
	}

	void Actor::AttachActor(std::shared_ptr<Actor> actor)
	{
		actor->SetParent(actor);
		m_children.emplace_back(std::move(actor));
	}

	void Actor::DetachActor(std::shared_ptr<Actor> actor)
	{
		auto actorIt = std::find(m_children.begin(), m_children.end(), actor);
		if (actorIt != m_children.end())
		{
			actor->SetParent(nullptr);
			m_children.erase(actorIt);
		}
	}

}
