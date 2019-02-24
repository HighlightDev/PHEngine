#include "Engine.h"

Engine::Engine()
   : m_interThreadMgr()
   , m_scene(m_interThreadMgr)
   , m_sceneRenderer(m_interThreadMgr, &m_scene)
{
   m_scene.AddTestActors();
}

Engine::~Engine()
{
}

void Engine::TickWindow()
{
   /* GAME THREAD*/
   {
      // This should be executed on game thread

      SPIN_GAME_THREAD_JOBS(m_interThreadMgr);
      m_scene.Tick_GameThread(0.05f);
   }

   /* RENDER THREAD */
   {
      // This should be executed on render thread

      SPIN_RENDER_THREAD_JOBS(m_interThreadMgr);
      m_sceneRenderer.RenderScene_RenderThread();
   }
}

void Engine::MouseMove()
{
   m_scene.CameraRotate();
}

void Engine::KeyDown()
{
   m_scene.CameraMove();
}