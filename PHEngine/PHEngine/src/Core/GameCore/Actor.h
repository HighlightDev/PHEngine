#pragma once

#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

#include "Core/GameCore/Components/Component.h"
#include "Core/GameCore/Components/SceneComponent.h"

namespace Game
{

	class Actor
	{
	private:

		std::shared_ptr<Game::SceneComponent> m_rootComponent;

	protected:

		std::vector<std::shared_ptr<Actor>> m_children;

		std::vector<std::shared_ptr<Game::Component>> m_allComponents;

		std::shared_ptr<Actor> m_parent;

	public:

		Actor(Game::SceneComponent* rootComponent);

		virtual ~Actor();

		// Tick is executed on game thread
		void Tick(float deltaTime);

		void AddComponent(std::shared_ptr<Game::Component> component);

		void RemoveComponent(std::shared_ptr<Game::Component> component);

		void SetParent(std::shared_ptr<Actor> actor);

		void AttachActor(std::shared_ptr<Actor> actor);

		void DetachActor(std::shared_ptr<Actor> actor);

		inline std::shared_ptr<Game::SceneComponent>& GetRootComponent() {

			return m_rootComponent;
		}

		// If root component has dirty transformation -> update it and all attached actors + children components
		void UpdateRootComponentTransform();

		// If components from list is scene component -> check if it has dity transformation, and if it does -> update it
		void UpdateComponentsTransform();
	};

}

