#pragma once

#include "ComponentType.h"

#include <memory>
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
		Actor* m_owner;

	public:

		Component();

		virtual ~Component();

      virtual uint64_t GetComponentType() const;

		// Game thread tick
		virtual void Tick(float deltaTime);

		void SetOwner(Actor* ownerActor);

      void RemoveOwner();

      inline Actor* GetOwner() const;

      Actor* GetBaseOwner() const;

	};

   inline Actor* Component::GetOwner() const
   {
      return m_owner;
   }

}

