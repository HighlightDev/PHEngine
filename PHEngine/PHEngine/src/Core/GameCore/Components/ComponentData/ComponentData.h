#pragma once

#include "Core/GameCore/Components/ComponentType.h"

namespace Game
{

	struct ComponentData
	{

		ComponentData() { }

		virtual ComponentType GetType() = 0;
	};

}