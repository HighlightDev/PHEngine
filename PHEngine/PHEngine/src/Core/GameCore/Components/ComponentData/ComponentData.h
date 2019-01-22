#pragma once

#include "Core/GameCore/Components/ComponentType.h"

namespace Game
{

	namespace Components
	{
		struct ComponentData
		{

			ComponentData() { }

			virtual ComponentType GetType() = 0;
		};
	}
}