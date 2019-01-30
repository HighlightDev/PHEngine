#include "Engine.h"

Engine::Engine()
{

}

Engine::~Engine()
{
}

void Engine::TickWindow()
{
   // This should be on game thread
   scene.Tick(0.05f);

   // This should be on render thread
   scene.Render();
}

void Engine::MouseMove()
{
	scene.CameraRotate();
}