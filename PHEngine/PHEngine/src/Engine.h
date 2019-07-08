#pragma once

#include "Core/GameCore/Scene.h"
#include "Core/GraphicsCore/Renderer/DeferredShadingSceneRenderer.h"

#include <thread>
#include <chrono>

using namespace Game;
using namespace Graphics::Renderer;

class Engine
{

private:

   using Clock_t = std::chrono::high_resolution_clock;

private:

   static constexpr double InvLimitFPS = 1.0 / 60.0;

   // TODO: INSIDE WORLD

   InterThreadCommunicationMgr m_interThreadMgr;

   Scene m_scene;

   DeferredShadingSceneRenderer m_sceneRenderer;

   std::thread m_gameThread;

   typename Clock_t::time_point mLastRenderThreadPulseTime;
   double mRenderThreadDeltaTimeSeconds;

   typename Clock_t::time_point mLastGameThreadPulseTime;
   double mGameThreadDeltaTimeSeconds;
   double mGameThreadSumDeltaTimeSec;

public:

	Engine();

	~Engine();

   void PostConstructorInitialize();

   void GameThreadPulse();

   void RenderThreadPulse();

	void TickWindow();

	void MouseMove();

   void KeyDown();

   double GetRenderThreadDeltaTime() const;
   double GetGameThreadDeltaTime() const;

#if DEBUG

   void PushFrame();

   void RecompileAllShaders();

#endif

private:

   double GetRenderThreadDeltaSeconds() const;
   double GetGameThreadDeltaSeconds() const;

};

