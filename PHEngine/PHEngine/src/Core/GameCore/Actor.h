#pragma once

#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

#include "Core/GameCore/Components/Component.h"

using namespace Game::Components;

namespace Game
{

	class Actor
	{
	private:

	protected:

		std::vector<std::shared_ptr<Component>> m_allComponents;

	public:
		Actor();
		virtual ~Actor();

		// Tick is executed on game thread
		void Tick(float deltaTime);

		// Render is executed on render thread
		void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, float deltaTime);

		void AddComponent(std::shared_ptr<Component> component);

		void RemoveComponent(std::shared_ptr<Component> component);
	};

}

