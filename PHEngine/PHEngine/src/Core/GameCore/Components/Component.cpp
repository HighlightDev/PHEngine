#include "Component.h"
#include "Core/GameCore/Actor.h"

#include <algorithm>

namespace Game
{

	Component::Component()
		: m_owner(nullptr)
	{

	}

	Component::~Component()
	{

	}

	void Component::Tick(float deltaTime)
	{

	}

	void Component::SetOwner(Actor* ownerActor)
	{
		m_owner = ownerActor;
	}

   void Component::RemoveOwner()
   {
      m_owner = nullptr;
   }

}