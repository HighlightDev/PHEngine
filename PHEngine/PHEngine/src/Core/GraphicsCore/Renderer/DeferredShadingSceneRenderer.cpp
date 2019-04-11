#include "DeferredShadingSceneRenderer.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/GraphicsCore/SceneProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/GlobalProperties.h"
#include "Core/GameCore/CameraBase.h"
#include "Core/GraphicsCore/Common/ScreenQuad.h"
#include "Core/GameCore/ShaderImplementation/DeferredShader.h"
#include "Core/GameCore/GlobalProperties.h"
#include "Core/GraphicsCore/SceneProxy/SkeletalMeshSceneProxy.h"

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

      DeferredShadingSceneRenderer::DeferredShadingSceneRenderer(InterThreadCommunicationMgr& interThreadMgr, Scene* const scene)
         : m_interThreadMgr(interThreadMgr)
         , m_scene(scene)
         , m_gbuffer(std::make_unique<DeferredShadingGBuffer>(GlobalProperties::GetInstance()->GetInputData().GetWindowWidth(), GlobalProperties::GetInstance()->GetInputData().GetWindowHeight()))
      {
         const auto& folderManager = FolderManager::GetInstance();

         std::string deferredBaseShaderPath = folderManager->GetShadersPath() + "deferredBasePassVS.glsl" + "," + folderManager->GetShadersPath() + "deferredBasePassFS.glsl";
         std::string deferredLightShaderPath = folderManager->GetShadersPath() + "deferredLightPassVS.glsl" + "," + folderManager->GetShadersPath() + "deferredLightPassFS.glsl";
         m_deferredBaseShader = std::dynamic_pointer_cast<DeferredShader>(ShaderPool::GetInstance()->template GetOrAllocateResource<DeferredShader>(deferredBaseShaderPath));
         m_deferredLightShader = std::dynamic_pointer_cast<DeferredLightShader>(ShaderPool::GetInstance()-> template GetOrAllocateResource<DeferredLightShader>(deferredLightShaderPath));
      }

      DeferredShadingSceneRenderer::~DeferredShadingSceneRenderer()
		{
		}

      void DeferredShadingSceneRenderer::DeferredBasePass_RenderThread(std::vector<std::shared_ptr<PrimitiveSceneProxy>>& deferredPrimitives, const glm::mat4& viewMatrix)
      {
         {                                                                                                                                   
            // Deferred shading collect info
            m_gbuffer->BindDeferredGBuffer();
            auto deferredShadingShader = m_deferredBaseShader;

            deferredShadingShader->ExecuteShader();
            for (auto& proxy : deferredPrimitives)
            {
               const glm::mat4& worldMatrix = proxy->GetMatrix();

               uint64_t type = proxy->GetComponentType();

               if (type == SKELETAL_MESH_COMPONENT)
               {
                  SkeletalMeshSceneProxy* skeletalProxy = static_cast<SkeletalMeshSceneProxy*>(proxy.get());
                  deferredShadingShader->SetSkinningMatrices(skeletalProxy->GetSkinningMatrices());
               }
               else
               {
                  deferredShadingShader->SetNotSkeletalMesh();
               }

               deferredShadingShader->SetTransformMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);

               proxy->GetAlbedo()->BindTexture(0);
               proxy->GetNormalMap()->BindTexture(1);
               deferredShadingShader->SetAlbedoTextureSlot(0);
               deferredShadingShader->SetNormalTextureSlot(1);
               proxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
            }
            deferredShadingShader->StopShader();

            m_gbuffer->UnbindDeferredGBuffer();
         }
      }

      void DeferredShadingSceneRenderer::DeferredLightPass_RenderThread()
      {
         {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0, 0, 0, 0);

            m_deferredLightShader->ExecuteShader();
            
            m_gbuffer->BindPositionTexture(0);
            m_gbuffer->BindAlbedoWithSpecularTexture(1);
            m_gbuffer->BindNormalTexture(2);

            m_deferredLightShader->SetGBufferPosition(0);
            m_deferredLightShader->SetGBufferAlbedoNSpecular(1);
            m_deferredLightShader->SetGBufferNormal(2);

            m_deferredLightShader->SetLightsInfo(m_scene->LightProxies);
            ScreenQuad::GetInstance()->GetBuffer()->RenderVAO(GL_TRIANGLES);
            m_deferredLightShader->StopShader();
         }
      }

      void DeferredShadingSceneRenderer::ForwardBasePass_RenderThread(std::vector<std::shared_ptr<PrimitiveSceneProxy>>& forwardedPrimitives, const glm::mat4& viewMatrix)
      {
         // Resolve depth buffer from gBuffer to default frame buffer

         int32_t windowWidth = GlobalProperties::GetInstance()->GetInputData().GetWindowWidth();
         int32_t windowHeight = GlobalProperties::GetInstance()->GetInputData().GetWindowHeight();

         glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gbuffer->GetFramebufferDesc());
         glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
         glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
         glBindFramebuffer(GL_FRAMEBUFFER, 0);

         for (auto& proxy : forwardedPrimitives)
         {
            proxy->Render(const_cast<glm::mat4&>(viewMatrix), m_scene->ProjectionMatrix);
         }
      }

      void DeferredShadingSceneRenderer::RenderScene_RenderThread()
      {
         glEnable(GL_DEPTH_TEST);

         const glm::mat4& viewMatrix = m_scene->GetCamera()->GetViewMatrix();

         std::vector<std::shared_ptr<PrimitiveSceneProxy>> drawDeferredShadedPrimitives;
         std::vector<std::shared_ptr<PrimitiveSceneProxy>> drawForwardShadedPrimitives;

         for (auto& proxy : m_scene->SceneProxies)
         {
            if (proxy->IsDeferred())
               drawDeferredShadedPrimitives.push_back(proxy);
            else
               drawForwardShadedPrimitives.push_back(proxy);
         }

         const bool bIsForwardShadedPrimitives = drawForwardShadedPrimitives.size() > 0;

         DeferredBasePass_RenderThread(drawDeferredShadedPrimitives, viewMatrix);

         DeferredLightPass_RenderThread();

         if (bIsForwardShadedPrimitives)
         {
            ForwardBasePass_RenderThread(drawForwardShadedPrimitives, viewMatrix);
         }

      }
	}
}