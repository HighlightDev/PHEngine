#pragma once

#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

#include "Core/GameCore/Components/Component.h"
#include "Core/GameCore/Components/SceneComponent.h"
#include "Core/GameCore/Components/InputComponent.h"

namespace Game
{

	class Actor
	{
	private:

		std::shared_ptr<Game::SceneComponent> m_rootComponent;

	protected:

      std::vector<std::shared_ptr<Actor>> m_children;

		std::shared_ptr<Actor> m_parent;

      std::shared_ptr<InputComponent> m_inputComponent;

	public:

      std::vector<std::shared_ptr<Game::Component>> m_allComponents;

		Actor(Game::SceneComponent* rootComponent = nullptr);

		virtual ~Actor();

		// Tick is executed on game thread
		virtual void Tick(float deltaTime);

      void AddInputComponent(std::shared_ptr<InputComponent> inputComponent);

		void AddComponent(std::shared_ptr<Game::Component> component);

		void RemoveComponent(std::shared_ptr<Game::Component> component);

		void SetParent(std::shared_ptr<Actor> actor);

		void AttachActor(std::shared_ptr<Actor> actor);

		void DetachActor(std::shared_ptr<Actor> actor);

		inline std::shared_ptr<Game::SceneComponent> GetRootComponent() {

			return m_rootComponent;
		}

      inline std::shared_ptr<InputComponent> GetInputComponent()
      {
         return m_inputComponent;
      }

		// If root component has dirty transformation -> update it and all attached actors + children components
		void UpdateRootComponentTransform();

		// If components from list is scene component -> check if it has dity transformation, and if it does -> update it
		void UpdateComponentsTransform();

      // When primitive component is removed, all primitive components which are alive and have index greater than removed component's index should do proxy index offset (-1)
      void RemoveComponentIndexOffset(size_t removedProxyIndex);
	};

}

