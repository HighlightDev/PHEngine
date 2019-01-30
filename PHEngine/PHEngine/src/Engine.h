#pragma once

#include "Core/GameCore/Scene.h"

using namespace Game;

class Engine
{
   // TODO: INSIDE WORLD
   Scene scene;

public:
	Engine();

	~Engine();

	void TickWindow();

	void MouseMove();
};

