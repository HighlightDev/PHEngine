#include "Engine.h"

Engine::Engine()
   : m_scene()
   , m_sceneRenderer(&m_scene)
{
   m_scene.AddTestActors();
}

Engine::~Engine()
{
}

void Engine::TickWindow()
{
   // This should be on game thread
   m_scene.Tick_GameThread(0.05f);

   // This should be on render thread
   m_sceneRenderer.BasePassRender_RenderThread();
}

void Engine::MouseMove()
{
   m_scene.CameraRotate();
}