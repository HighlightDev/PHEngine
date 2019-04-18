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
#include "Core/GraphicsCore/SceneProxy/DirectionalLightSceneProxy.h"
#include "Core/GraphicsCore/SceneProxy/PointLightSceneProxy.h"
#include "Core/GraphicsCore/Shadow/ProjectedShadowInfo.h"

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

         std::string deferredNonSkeletalBaseShaderPath = folderManager->GetShadersPath() + "deferredNonSkeletalBasePassVS.glsl" + "," + folderManager->GetShadersPath() + "deferredBasePassFS.glsl";
         std::string deferredSkeletalBaseShaderPath = folderManager->GetShadersPath() + "deferredSkeletalBasePassVS.glsl" + "," + folderManager->GetShadersPath() + "deferredBasePassFS.glsl";
         std::string deferredLightShaderPath = folderManager->GetShadersPath() + "deferredLightPassVS.glsl" + "," + folderManager->GetShadersPath() + "deferredLightPassFS.glsl";

         m_deferredBaseShaderNonSkeletal = std::dynamic_pointer_cast<DeferredShader<false>>(ShaderPool::GetInstance()->template GetOrAllocateResource<DeferredShader<false>>(deferredNonSkeletalBaseShaderPath));
         m_deferredBaseShaderSkeletal = std::dynamic_pointer_cast<DeferredShader<true>>(ShaderPool::GetInstance()->template GetOrAllocateResource<DeferredShader<true>>(deferredSkeletalBaseShaderPath));
         m_deferredLightShader = std::dynamic_pointer_cast<DeferredLightShader>(ShaderPool::GetInstance()-> template GetOrAllocateResource<DeferredLightShader>(deferredLightShaderPath));
      }

      DeferredShadingSceneRenderer::~DeferredShadingSceneRenderer()
      {
      }

      void DeferredShadingSceneRenderer::DepthPass(std::vector<PrimitiveSceneProxy*>& shadowDependentPrimitives, std::vector<LightSceneProxy*>& lightSourcesProxy)
      {

         for (auto& lightProxy : lightSourcesProxy)
         {
            const ProjectedShadowInfo* const shadowInfo = lightProxy->GetShadowInfo();
            if (shadowInfo)
            {
               if (lightProxy->GetLightProxyType() == LightSceneProxyType::DIR_LIGHT)
               {
                  const DirectionalLightSceneProxy* dirLightSceneProxy = static_cast<DirectionalLightSceneProxy*>(lightProxy);

                  // bind depth shader
                  for (auto& primitive : shadowDependentPrimitives)
                  {
                     // WHAT IF PRIMITIVE IS SKELETAL MESH????!!!!

                     const auto& worldMatrix = primitive->GetMatrix();
                     const auto& viewMatrix = shadowInfo->ShadowViewMatrices[0];
                     const auto& projectionMatrix = shadowInfo->ShadowProjectionMatrices[0];

                     primitive->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
                  }
                  // unbind depth shader
               }
               else if (lightProxy->GetLightProxyType() == LightSceneProxyType::POINT_LIGHT)
               {
                  const PointLightSceneProxy* pointLightSceneProxy = static_cast<PointLightSceneProxy*>(lightProxy);

               }
            }
         }

      }

      void DeferredShadingSceneRenderer::DeferredBasePass_RenderThread(std::vector<PrimitiveSceneProxy*>& deferredPrimitives, const glm::mat4& viewMatrix)
      {
         {
            std::vector<PrimitiveSceneProxy*> skeletalPrimitives, nonSkeletalPrimitives;
            for (auto& proxy : deferredPrimitives)
            {
               if (proxy->GetComponentType() == SKELETAL_MESH_COMPONENT)
               {
                  skeletalPrimitives.push_back(proxy);
               }
               else
               {
                  nonSkeletalPrimitives.push_back(proxy);
               }
            }

            // Deferred shading collect info
            m_gbuffer->BindDeferredGBuffer();

            if (skeletalPrimitives.size() > 0)
            {
               m_deferredBaseShaderSkeletal->ExecuteShader();
               for (auto& proxy : skeletalPrimitives)
               {
                  SkeletalMeshSceneProxy* skeletalProxy = static_cast<SkeletalMeshSceneProxy*>(proxy);
                  m_deferredBaseShaderSkeletal->SetSkinningMatrices(skeletalProxy->GetSkinningMatrices());
                  const glm::mat4& worldMatrix = proxy->GetMatrix();
                  m_deferredBaseShaderSkeletal->SetTransformMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);
                  proxy->GetAlbedo()->BindTexture(0);
                  proxy->GetNormalMap()->BindTexture(1);
                  m_deferredBaseShaderSkeletal->SetAlbedoTextureSlot(0);
                  m_deferredBaseShaderSkeletal->SetNormalTextureSlot(1);
                  proxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
               }
               m_deferredBaseShaderSkeletal->StopShader();
            }

            if (nonSkeletalPrimitives.size() > 0)
            {
               for (auto& proxy : nonSkeletalPrimitives)
               {
                  m_deferredBaseShaderNonSkeletal->ExecuteShader();
                  const glm::mat4& worldMatrix = proxy->GetMatrix();
                  m_deferredBaseShaderNonSkeletal->SetTransformMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);
                  proxy->GetAlbedo()->BindTexture(0);
                  proxy->GetNormalMap()->BindTexture(1);
                  m_deferredBaseShaderNonSkeletal->SetAlbedoTextureSlot(0);
                  m_deferredBaseShaderNonSkeletal->SetNormalTextureSlot(1);
                  proxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
               }
            }

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

      void DeferredShadingSceneRenderer::ForwardBasePass_RenderThread(std::vector<PrimitiveSceneProxy*>& forwardedPrimitives, const glm::mat4& viewMatrix)
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

         std::vector<PrimitiveSceneProxy*> drawDeferredShadedPrimitives;
         std::vector<PrimitiveSceneProxy*> drawForwardShadedPrimitives;

         for (auto& proxy : m_scene->SceneProxies)
         {
            if (proxy->IsDeferred())
               drawDeferredShadedPrimitives.push_back(proxy.get());
            else
               drawForwardShadedPrimitives.push_back(proxy.get());
         }

         const bool bIsForwardShadedPrimitives = drawForwardShadedPrimitives.size() > 0;

         DeferredBasePass_RenderThread(drawDeferredShadedPrimitives, viewMatrix);

         DeferredLightPass_RenderThread();

         if (bIsForwardShadedPrimitives)
         {
            ForwardBasePass_RenderThread(drawForwardShadedPrimitives, viewMatrix);
         }

      }

      void DeferredShadingSceneRenderer::DebugFramePanelsPass()
      {
         glDisable(GL_DEPTH_TEST);
         m_textureRenderer.RenderFrames();
      }

      void DeferredShadingSceneRenderer::PushRenderTargetToTextureRenderer()
      {
         m_textureRenderer.PushDebugRenderTarget();
      }
	}
}