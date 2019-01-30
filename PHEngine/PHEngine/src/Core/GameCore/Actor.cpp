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

	void Actor::UpdateRootComponentTransform()
	{
		if (m_rootComponent)
		{
			// Root component and all attached objects to this actor must update their transforms
			if (m_rootComponent->GetIsTransformationDirty())
			{
				// Update root component with parent transform matrix
				{
					glm::mat4 parentRelativeMatrix(1);	// identity matrix

					if (m_parent)
						parentRelativeMatrix = m_parent->GetRootComponent()->GetRelativeMatrix();

					m_rootComponent->UpdateRelativeMatrix(parentRelativeMatrix);
				}

				// Update all components that have transformation
				glm::mat4 rootRelativeMatrix = std::move(m_rootComponent->GetRelativeMatrix());
				for (auto& component : m_allComponents)
				{
					if (component->GetComponentType() & ComponentType::SCENE_COMPONENT)
					{
						SceneComponent* sceneComp = static_cast<SceneComponent*>(component.get());
						sceneComp->UpdateRelativeMatrix(rootRelativeMatrix);
					}
				}
			}
			else // If root component wasn't updated then just check if component has dirty transform
			{
				UpdateComponentsTransform();
			}
		}
	}

	void Actor::UpdateComponentsTransform() 
	{
		if (m_allComponents.size() > 0)
		{
			glm::mat4 parentRelativeMatrix(1);	// identity matrix

			if (m_parent)
				parentRelativeMatrix = m_parent->GetRootComponent()->GetRelativeMatrix();

			// Update all components that have transformation
			glm::mat4 rootRelativeMatrix = std::move(m_rootComponent->GetRelativeMatrix());
			for (auto& component : m_allComponents)
			{
				if (component->GetComponentType() & ComponentType::SCENE_COMPONENT)
				{
					SceneComponent* sceneComp = static_cast<SceneComponent*>(component.get());
					if (sceneComp->GetIsTransformationDirty())
					{
						sceneComp->UpdateRelativeMatrix(rootRelativeMatrix);
					}
				}
			}
		}
	}

	void Actor::Tick(float deltaTime)
	{
		UpdateRootComponentTransform();

		m_rootComponent->Tick(deltaTime);

		for (auto& component : m_allComponents)
		{
			// tick all children components
			component->Tick(deltaTime);
		}

		for (auto& actor : m_children)
		{
			// tick all attached actors
			actor->Tick(deltaTime);
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
