#pragma once

#include <stdint.h>
#include <glm/mat4x4.hpp>

#include "Core/GameCore/Scene.h"
#include "Core/GameCore/ShaderImplementation/DeferredShader.h"
#include "Core/GameCore/ShaderImplementation/DeferredLightShader.h"
#include "Core/GraphicsCore/Renderer/DeferredShadingGBuffer.h"
#include "Core/InterThreadCommunicationMgr.h"

using namespace Game::ShaderImpl;
using namespace Game;
using namespace Thread;

namespace Graphics
{
	namespace Renderer
	{

		class DeferredShadingSceneRenderer
		{
         InterThreadCommunicationMgr& m_interThreadMgr;

         /* Scene to render */
         Scene* const m_scene;

         std::unique_ptr<DeferredShadingGBuffer> m_gbuffer;

         std::shared_ptr<DeferredShader<false>> m_deferredBaseShaderNonSkeletal;
         std::shared_ptr<DeferredShader<true>> m_deferredBaseShaderSkeletal;

         std::shared_ptr<DeferredLightShader> m_deferredLightShader;

      public:

         DeferredShadingSceneRenderer(InterThreadCommunicationMgr& interThreadMgr, Scene* const scene);

			~DeferredShadingSceneRenderer();

         void RenderScene_RenderThread();

         void DeferredLightPass_RenderThread();

         void DeferredBasePass_RenderThread(std::vector<PrimitiveSceneProxy*>& deferredPrimitives, const glm::mat4& viewMatrix);

         void ForwardBasePass_RenderThread(std::vector<PrimitiveSceneProxy*>& forwardedPrimitives, const glm::mat4& viewMatrix);

         void DepthPass();

		};

	}
}

