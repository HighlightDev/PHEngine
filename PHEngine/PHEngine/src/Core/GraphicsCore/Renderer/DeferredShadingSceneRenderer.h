#pragma once

#include <stdint.h>

#include "Core/GameCore/Scene.h"
#include "Core/GameCore/ShaderImplementation/DeferredShader.h"
#include "Core/GraphicsCore/Renderer/DeferredShadingGBuffer.h"

using namespace Game::ShaderImpl;
using namespace Game;

namespace Graphics
{
	namespace Renderer
	{

		class DeferredShadingSceneRenderer
		{
         /* Scene to render */
         Scene* const m_scene;

         std::unique_ptr<DeferredShadingGBuffer> m_gbuffer;

         std::shared_ptr<DeferredShader> m_shader;

      public:

         DeferredShadingSceneRenderer(Scene* const scene);

			~DeferredShadingSceneRenderer();

         void BasePassRender_RenderThread();

		};

	}
}

