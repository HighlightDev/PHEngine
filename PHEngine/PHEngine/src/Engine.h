#pragma once

#include "Core/GameCore/Scene.h"
#include "Core/GraphicsCore/Renderer/DeferredShadingSceneRenderer.h"
#include <thread>

using namespace Game;
using namespace Graphics::Renderer;

class Engine
{
   // TODO: INSIDE WORLD

   InterThreadCommunicationMgr m_interThreadMgr;

   Scene m_scene;

   DeferredShadingSceneRenderer m_sceneRenderer;

   std::thread m_gameThread;

public:

	Engine();

	~Engine();

   void PostConstructorInitialize();

   void GameThreadPulse();

   void RenderThreadPulse();

	void TickWindow();

	void MouseMove();

   void KeyDown();

#if DEBUG

   void PushFrame();

   void RecompileAllShaders();

#endif
};

