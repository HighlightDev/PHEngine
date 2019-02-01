#pragma once

#include "Core/GameCore/Scene.h"
#include "Core/GraphicsCore/Renderer/DeferredShadingSceneRenderer.h"

using namespace Game;
using namespace Graphics::Renderer;

class Engine
{
   // TODO: INSIDE WORLD

   Scene m_scene;
   DeferredShadingSceneRenderer m_sceneRenderer;

public:
	Engine();

	~Engine();

	void TickWindow();

	void MouseMove();
};

