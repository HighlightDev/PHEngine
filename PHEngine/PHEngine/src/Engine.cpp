#include "Engine.h"

#include "Core/GameCore/Event/TEvent.h"

Engine::Engine()
   : m_interThreadMgr()
   , m_scene(m_interThreadMgr)
   , m_sceneRenderer(m_interThreadMgr, &m_scene)
{
   m_scene.AddTestActors();

   m_gameThread = std::thread(std::bind(&Engine::GameThreadPulse, this));
   m_gameThread.detach();
}

Engine::~Engine()
{
}

void Engine::GameThreadPulse()
{
   while (true)
   {
      /* GAME THREAD*/
      {
         // This should be executed on game thread

         SPIN_GAME_THREAD_JOBS(m_interThreadMgr);
         m_scene.Tick_GameThread(0.0005f);
      }
   }
}

void Engine::RenderThreadPulse()
{
   /* RENDER THREAD */
   {
      // This should be executed on render thread

      SPIN_RENDER_THREAD_JOBS(m_interThreadMgr);
      m_sceneRenderer.RenderScene_RenderThread();
   }
}

void Engine::TickWindow()
{
   RenderThreadPulse();
}

void Engine::MouseMove()
{
   m_scene.CameraRotate();
}

void Engine::KeyDown()
{
   //m_sceneRenderer.PushRenderTargetToTextureRenderer();
   m_scene.CameraMove();
}