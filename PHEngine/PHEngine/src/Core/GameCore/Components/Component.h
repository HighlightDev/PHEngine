#pragma once

#include "ComponentType.h"

#include <memory>
#include <mutex>
#include <vector>
#include <glm/vec3.hpp>

namespace Game
{

	class Actor;
	// This is the base abstract class
	// of all components which could be 
	// picked by actor
	class Component
	{
		std::mutex m_lock;

		Actor* m_owner;

	public:

		Component();

		virtual ~Component();

      virtual ComponentType GetComponentType() const;

		// Game thread tick
		virtual void Tick(float deltaTime);

		void SetOwner(Actor* ownerActor);

      void RemoveOwner();

      inline Actor* GetOwner();
	};

   inline Actor* Component::GetOwner()
   {
      return m_owner;
   }

}

