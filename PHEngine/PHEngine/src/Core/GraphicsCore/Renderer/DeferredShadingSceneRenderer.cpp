#include "DeferredShadingSceneRenderer.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/GraphicsCore/PrimitiveProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/GlobalProperties.h"
#include "Core/GameCore/CameraBase.h"
#include "Core/GraphicsCore/Common/ScreenQuad.h"
#include "Core/GameCore/ShaderImplementation/DeferredShader.h"
#include "Core/GameCore/GlobalProperties.h"

#include <gl/glew.h>
#include <iostream>

using namespace Resources;
using namespace Common;
using namespace Graphics;
using namespace Graphics::Renderer;
using namespace Graphics::Proxy;

namespace Graphics
{
	namespace Renderer
	{

      DeferredShadingSceneRenderer::DeferredShadingSceneRenderer(Scene* const scene)
         : m_scene(scene)
         , m_gbuffer(std::make_unique<DeferredShadingGBuffer>(GlobalProperties::GetInstance()->GetInputData().GetWindowWidth(), GlobalProperties::GetInstance()->GetInputData().GetWindowHeight()))
      {
         std::string deferredShaderPath = FolderManager::GetInstance()->GetShadersPath() + "deferredBasePassVS.glsl" + "," + FolderManager::GetInstance()->GetShadersPath() + "deferredBasePassFS.glsl";
         m_shader = std::dynamic_pointer_cast<DeferredShader>(ShaderPool::GetInstance()->template GetOrAllocateResource<DeferredShader>(deferredShaderPath));
      }

      DeferredShadingSceneRenderer::~DeferredShadingSceneRenderer()
		{
		}

      void DeferredShadingSceneRenderer::BasePassRender_RenderThread()
      {
         glEnable(GL_DEPTH_TEST);

         glm::mat4 viewMatrix = m_scene->GetCamera()->GetViewMatrix();

         std::vector<std::shared_ptr<PrimitiveSceneProxy>> drawDeferredShadedPrimitives;
         std::vector<std::shared_ptr<PrimitiveSceneProxy>> drawForwardShadedPrimitives;

         for (auto& proxy : m_scene->SceneProxies)
         {
            if (proxy->IsDeferred())
               drawDeferredShadedPrimitives.push_back(proxy);
            else
               drawForwardShadedPrimitives.push_back(proxy);
         }

         {
            // Deferred shading collect info
            m_gbuffer->BindDeferredGBuffer();
            auto deferredShadingShader = m_shader;

            deferredShadingShader->ExecuteShader();
            for (auto& proxy : drawDeferredShadedPrimitives)
            {
               const glm::mat4& worldMatrix = proxy->GetMatrix();
               deferredShadingShader->SetTransformMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);

               proxy->GetAlbedo()->BindTexture(0);
               deferredShadingShader->SetAlbedoTextureSlot(0);

               proxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
            }
            deferredShadingShader->StopShader();

            m_gbuffer->UnbindDeferredGBuffer();
         }

         {
            // Forward shading
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0, 0, 0, 0);

            ScreenQuad::GetInstance()->GetResolveTexShader()->ExecuteShader();
            m_gbuffer->BindNormalTexture(0);
            ScreenQuad::GetInstance()->GetResolveTexShader()->SetTextureSlot(0);
            ScreenQuad::GetInstance()->GetBuffer()->RenderVAO(GL_TRIANGLES);
            ScreenQuad::GetInstance()->GetResolveTexShader()->StopShader();

            // Resolve depth buffer from gBuffer to default frame buffer

            int32_t windowWidth = GlobalProperties::GetInstance()->GetInputData().GetWindowWidth();
            int32_t windowHeight = GlobalProperties::GetInstance()->GetInputData().GetWindowHeight();

            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gbuffer->GetFramebufferDesc());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            for (auto& proxy : drawForwardShadedPrimitives)
            {
               const glm::mat4& worldMatrix = proxy->GetMatrix();
               proxy->Render(viewMatrix, m_scene->ProjectionMatrix);
            }

         }
      }
	}
}