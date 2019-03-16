#pragma once

#include "Core/GameCore/Components/ComponentType.h"

namespace Game
{

	struct ComponentData
	{

		ComponentData() { }

		virtual size_t GetType() = 0;
	};

}