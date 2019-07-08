#include "Engine.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"

Engine::Engine()
   : m_interThreadMgr()
   , m_scene(m_interThreadMgr)
   , m_sceneRenderer(m_interThreadMgr, &m_scene)
   , mLastRenderThreadPulseTime(Clock_t::now())
   , mRenderThreadDeltaTimeSeconds()
   , mLastGameThreadPulseTime(Clock_t::now())
   , mGameThreadDeltaTimeSeconds()
   , mGameThreadSumDeltaTimeSec()
{
   PostConstructorInitialize();
}

Engine::~Engine()
{
}

void Engine::PostConstructorInitialize()
{
   m_scene.AddTestActors();

   m_scene.PostConstructorInitialize();
   m_sceneRenderer.PostConstructorInitialize();

   m_gameThread = std::thread(std::bind(&Engine::GameThreadPulse, this));
   m_gameThread.detach();
}

void Engine::GameThreadPulse()
{
   while (true)
   {
      /* GAME THREAD*/
      {
         mGameThreadDeltaTimeSeconds = GetGameThreadDeltaSeconds();
         mGameThreadSumDeltaTimeSec += mGameThreadDeltaTimeSeconds;

         SPIN_GAME_THREAD_JOBS(m_interThreadMgr);
         if (mGameThreadSumDeltaTimeSec >= InvLimitFPS) // 1 / 60 of a second
         {
            // This should be executed on game thread
            m_scene.Tick_GameThread(static_cast<float>(mGameThreadDeltaTimeSeconds));
            mLastGameThreadPulseTime = Clock_t::now();
            mGameThreadSumDeltaTimeSec = 0.0;
         }
      }
   }
}

void Engine::RenderThreadPulse()
{
   /* RENDER THREAD */
   {
      mRenderThreadDeltaTimeSeconds = GetRenderThreadDeltaSeconds();
      
      // This should be executed on render thread
      SPIN_RENDER_THREAD_JOBS(m_interThreadMgr);
      m_sceneRenderer.RenderScene_RenderThread();
      mLastRenderThreadPulseTime = Clock_t::now();
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
   m_scene.CameraMove();
}

double Engine::GetRenderThreadDeltaSeconds() const
{
   Clock_t::duration deltaTime = Clock_t::now() - mLastRenderThreadPulseTime;
   const double invFromNanoToSec = 0.000000001;
   return static_cast<double>(deltaTime.count()) * invFromNanoToSec;
}

double Engine::GetGameThreadDeltaSeconds() const
{
   Clock_t::duration deltaTime = Clock_t::now() - mLastGameThreadPulseTime;
   const double invFromNanoToSec = 0.000000001;
   return static_cast<double>(deltaTime.count()) * invFromNanoToSec;
}

double Engine::GetRenderThreadDeltaTime() const
{
   return mRenderThreadDeltaTimeSeconds;
}

double Engine::GetGameThreadDeltaTime() const
{
   return mGameThreadDeltaTimeSeconds;
}

#if DEBUG

void Engine::PushFrame()
{
   m_sceneRenderer.PushRenderTargetToTextureRenderer();
}

void Engine::RecompileAllShaders()
{
   Resources::ShaderPool::GetInstance()->RecompileShaders();
}

#endif