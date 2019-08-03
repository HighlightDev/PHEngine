#include "DeferredShadingSceneRenderer.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"
#include "Core/ResourceManagerCore/Pool/CompositeShaderPool.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/GraphicsCore/SceneProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/GlobalProperties.h"
#include "Core/GameCore/ICamera.h"
#include "Core/GraphicsCore/Common/ScreenQuad.h"
#include "Core/GameCore/ShaderImplementation/DeferredShader.h"
#include "Core/GameCore/GlobalProperties.h"
#include "Core/GraphicsCore/SceneProxy/SkeletalMeshSceneProxy.h"
#include "Core/GraphicsCore/SceneProxy/SkyboxSceneProxy.h"
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
using namespace Graphics::OpenGL;

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

         ShaderParams shaderParams1("DeferredNonSkeletalBase Shader", folderManager->GetShadersPath() + "deferredNonSkeletalBasePassVS.glsl", folderManager->GetShadersPath() + "deferredBasePassFS.glsl", "", "", "", "");
         ShaderParams shaderParams2("DeferredSkeletalBase Shader", folderManager->GetShadersPath() + "deferredSkeletalBasePassVS.glsl", folderManager->GetShadersPath() + "deferredBasePassFS.glsl", "", "", "", "");
         ShaderParams shaderParams3("DeferredLight Shader", folderManager->GetShadersPath() + "deferredLightPassVS.glsl", folderManager->GetShadersPath() + "deferredLightPassFS.glsl", "", "", "", "");
         ShaderParams shaderParams4("DepthSkeletal Shader", folderManager->GetShadersPath() + "basicShadowSkeletalVS.glsl", folderManager->GetShadersPath() + "basicShadowFS.glsl", "", "", "", "");
         ShaderParams shaderParams5("DepthNonSkeletal Shader", folderManager->GetShadersPath() + "basicShadowNonSkeletalVS.glsl", folderManager->GetShadersPath() + "basicShadowFS.glsl", "", "", "", "");
         ShaderParams shaderParams6("CubemapDepthSkeletal Shader", folderManager->GetShadersPath() + "cubemapShadowSkeletalVS.glsl", folderManager->GetShadersPath() + "cubemapShadowFS.glsl", folderManager->GetShadersPath() + "cubemapShadowGS.glsl", "", "", "");
         ShaderParams shaderParams7("CubemapDepthNonSkeletal Shader", folderManager->GetShadersPath() + "cubemapShadowNonSkeletalVS.glsl", folderManager->GetShadersPath() + "cubemapShadowFS.glsl", folderManager->GetShadersPath() + "cubemapShadowGS.glsl", "", "", "");

         m_deferredBaseShaderNonSkeletal = std::static_pointer_cast<DeferredShader<false>>(ShaderPool::GetInstance()->template GetOrAllocateResource<DeferredShader<false>>(shaderParams1));
         m_deferredBaseShaderSkeletal = std::static_pointer_cast<DeferredShader<true>>(ShaderPool::GetInstance()->template GetOrAllocateResource<DeferredShader<true>>(shaderParams2));
         m_deferredLightShader = std::static_pointer_cast<DeferredLightShader>(ShaderPool::GetInstance()-> template GetOrAllocateResource<DeferredLightShader>(shaderParams3));
         m_depthShaderSkeletal = std::static_pointer_cast<DepthShader<true>>(ShaderPool::GetInstance()->template GetOrAllocateResource<DepthShader<true>>(shaderParams4));
         m_depthShaderNonSkeletal = std::static_pointer_cast<DepthShader<false>>(ShaderPool::GetInstance()->template GetOrAllocateResource<DepthShader<false>>(shaderParams5));
         m_depthCubemapShaderSkeletal = std::static_pointer_cast<CubemapDepthShader<true>>(ShaderPool::GetInstance()->template GetOrAllocateResource< CubemapDepthShader<true>>(shaderParams6));
         m_depthCubemapShaderNonSkeletal = std::static_pointer_cast<CubemapDepthShader<false>>(ShaderPool::GetInstance()->template GetOrAllocateResource< CubemapDepthShader<true>>(shaderParams7));


         using texShared = PBRMaterial::ITextureShared;
         std::string diffuseTexPath = folderManager->GetAlbedoTexturePath() + "diffuse.png";
         std::string nmPath = folderManager->GetNormalMapPath() + "dummy_nm.png";
         texShared image = TexturePool::GetInstance()->GetOrAllocateResource(diffuseTexPath);
         texShared imageNM = TexturePool::GetInstance()->GetOrAllocateResource(nmPath);
         mMatSkelet = new PBRMaterial(image, imageNM, imageNM, imageNM, imageNM);

         std::string diffuseTexPath1 = folderManager->GetAlbedoTexturePath() + "city_house_2_Col.png";
         std::string nmPath1 = folderManager->GetNormalMapPath() + "city_house_2_Nor.png";
         texShared image1 = TexturePool::GetInstance()->GetOrAllocateResource(diffuseTexPath1);
         texShared imageNM1 = TexturePool::GetInstance()->GetOrAllocateResource(nmPath1);
         mMatStatic = new PBRMaterial(image1, imageNM1, imageNM, imageNM, imageNM);
        
         using staticShader_t = CompositeShader<StaticMeshVertexFactory, CompositeDeferredCollectShader, MaterialShaderImp<PBRMaterial>>;
         using skeletShader_t = CompositeShader<SkeletalMeshVertexFactory<3>, CompositeDeferredCollectShader, MaterialShaderImp<PBRMaterial>>;

         const auto shParams = ShaderParams("DeferredNonSkeletalBase Shader", FolderManager::GetInstance()->GetShadersPath() + "composite_shaders\\" + "deferredBaseVS.glsl", FolderManager::GetInstance()->GetShadersPath() + "composite_shaders\\" + "deferredBaseFS.glsl", "", "", "", "");

         TemplatedCompositeShaderParams<skeletShader_t> compositeParams(COMPOSITE_SHADER_TO_STR(SkeletalMeshVertexFactory<3>, CompositeDeferredCollectShader, MaterialShaderImp<PBRMaterial>), shParams);
         TemplatedCompositeShaderParams<staticShader_t> compositeParams1(COMPOSITE_SHADER_TO_STR(StaticMeshVertexFactory, CompositeDeferredCollectShader, MaterialShaderImp<PBRMaterial>), shParams);

         mTestShaderSkelet = std::static_pointer_cast<skeletShader_t>(CompositeShaderPool::GetInstance()->GetOrAllocateResource<skeletShader_t>(compositeParams));
         mTestShaderStatic = std::static_pointer_cast<staticShader_t>(CompositeShaderPool::GetInstance()->GetOrAllocateResource<staticShader_t>(compositeParams1));
      }

      DeferredShadingSceneRenderer::~DeferredShadingSceneRenderer()
      {
      }

      void DeferredShadingSceneRenderer::PostConstructorInitialize()
      {
         
      }

      std::vector<std::shared_ptr<DirectionalLightSceneProxy>> DeferredShadingSceneRenderer::RetrieveDirectionalLightProxies(const std::vector<std::shared_ptr<LightSceneProxy>>& lightSourcesProxy) const
      {
         std::vector<std::shared_ptr<DirectionalLightSceneProxy>> directionalLights;
         for (const auto& lightProxy : lightSourcesProxy)
         {
            if (lightProxy->GetLightProxyType() == LightSceneProxyType::DIR_LIGHT)
               directionalLights.push_back(std::static_pointer_cast<DirectionalLightSceneProxy>(lightProxy));
         }
         return directionalLights;
      }

      std::vector<std::shared_ptr<PointLightSceneProxy>> DeferredShadingSceneRenderer::RetrievePointLightProxies(const std::vector<std::shared_ptr<LightSceneProxy>>& lightSourcesProxy) const
      {
         std::vector<std::shared_ptr<PointLightSceneProxy>> pointLights;
         for (const auto& lightProxy : lightSourcesProxy)
         {
            if (lightProxy->GetLightProxyType() == LightSceneProxyType::POINT_LIGHT)
               pointLights.push_back(std::static_pointer_cast<PointLightSceneProxy>(lightProxy));
         }
         return pointLights;
      }

      void DeferredShadingSceneRenderer::DepthPass(std::vector<PrimitiveSceneProxy*>& shadowNonSkeletalMeshPrimitives, std::vector<PrimitiveSceneProxy*>& shadowSkeletalMeshPrimitives, const std::vector<std::shared_ptr<LightSceneProxy>>& lightSourcesProxy)
      {
         auto dirLightProxies = RetrieveDirectionalLightProxies(lightSourcesProxy);
         auto pointLightProxies = RetrievePointLightProxies(lightSourcesProxy);

         std::sort(dirLightProxies.begin(), dirLightProxies.end(),
            [](const std::shared_ptr<DirectionalLightSceneProxy>& firstProxy, const std::shared_ptr<DirectionalLightSceneProxy>& secondProxy)
         {
            bool result = false;
            const auto& shadowInfo1 = firstProxy->GetProjectedDirShadowInfo();
            const auto& shadowInfo2 = secondProxy->GetProjectedDirShadowInfo();
            if (shadowInfo1 && shadowInfo2)
            {
               const uint32_t desc1 = shadowInfo1->GetAtlasResource()->GetTextureDescriptor();
               const uint32_t desc2 = shadowInfo2->GetAtlasResource()->GetTextureDescriptor();
               result = desc1 > desc2;
            }
            else if (shadowInfo1 && !shadowInfo2)
            {
               result = true;
            }

            return result;
         });

         const auto firstDirLightProxyWithShadowInfo = std::find_if(dirLightProxies.begin(), dirLightProxies.end(), [](const std::shared_ptr<DirectionalLightSceneProxy>& proxy) { return proxy->GetProjectedDirShadowInfo() != nullptr; });
         uint32_t lastDirLightFramebufferDesc = std::numeric_limits<uint32_t>::max();

         for (auto& dirLightProxy : dirLightProxies)
         {
            DirectionalLightSceneProxy* lightPtr = dirLightProxy.get();

            ProjectedShadowInfo* const shadowInfo = lightPtr->GetProjectedDirShadowInfo();
            if (shadowInfo && shadowInfo->bMustUpdateShadowmap)
            {
               shadowInfo->BindShadowFramebuffer(shadowInfo->GetAtlasResource()->GetTextureDescriptor() != lastDirLightFramebufferDesc);
               lastDirLightFramebufferDesc = shadowInfo->GetAtlasResource()->GetTextureDescriptor();

               DirectionalLightSceneProxy* dirLightPtr = static_cast<DirectionalLightSceneProxy*>(lightPtr);

               if (shadowNonSkeletalMeshPrimitives.size() > 0) // Non - skeletal primitives
               {
                  m_depthShaderNonSkeletal->ExecuteShader();
                  for (auto& primitive : shadowNonSkeletalMeshPrimitives)
                  {
                     const auto& worldMatrix = primitive->GetMatrix();
                     const auto& viewMatrix = dirLightPtr->GetProjectedDirShadowInfo()->GetShadowViewMatrix();
                     const auto& projectionMatrix = dirLightPtr->GetProjectedDirShadowInfo()->GetShadowProjectionMatrix();

                     m_depthShaderNonSkeletal->SetTransformationMatrices(worldMatrix, viewMatrix, projectionMatrix);

                     primitive->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
                  }
                  m_depthShaderNonSkeletal->StopShader();
               }

               if (shadowSkeletalMeshPrimitives.size() > 0) // Skeletal primitives
               {
                  m_depthShaderSkeletal->ExecuteShader();
                  for (auto& primitive : shadowSkeletalMeshPrimitives)
                  {
                     SkeletalMeshSceneProxy* skeletalProxy = static_cast<SkeletalMeshSceneProxy*>(primitive);

                     const auto& worldMatrix = primitive->GetMatrix();
                     const auto& viewMatrix = dirLightPtr->GetProjectedDirShadowInfo()->GetShadowViewMatrix();
                     const auto& projectionMatrix = dirLightPtr->GetProjectedDirShadowInfo()->GetShadowProjectionMatrix();
                     m_depthShaderSkeletal->SetTransformationMatrices(worldMatrix, viewMatrix, projectionMatrix);
                     m_depthShaderSkeletal->SetSkinningMatrices(skeletalProxy->GetSkinningMatrices());

                     primitive->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
                  }
                  m_depthShaderSkeletal->StopShader();
               }

               glBindFramebuffer(GL_FRAMEBUFFER, 0);

               // Next frame shadow map will not be updated unless position of objects in the level are changed
               shadowInfo->bMustUpdateShadowmap = false;
            }
         }

         for (auto& pointLightProxy : pointLightProxies)
         {
            PointLightSceneProxy* pointLightPtr = static_cast<PointLightSceneProxy*>(pointLightProxy.get());

            const auto& shadowInfo = pointLightPtr->GetProjectedPointShadowInfo();
            if (shadowInfo)
            {
               shadowInfo->BindShadowFramebuffer(true); // every point light has it's own texture atlas 
               
               if (shadowNonSkeletalMeshPrimitives.size() > 0) // Non - skeletal primitives
               {
                  m_depthCubemapShaderNonSkeletal->ExecuteShader();
                  for (auto& primitive : shadowNonSkeletalMeshPrimitives)
                  {
                     const auto& worldMatrix = primitive->GetMatrix();
                     const auto& viewMatrices = shadowInfo->GetShadowViewMatrices();
                     const auto& projectionMatrices = shadowInfo->GetShadowProjectionMatrices();

                     m_depthCubemapShaderNonSkeletal->SetTransformationMatrices(worldMatrix, viewMatrices, projectionMatrices);
                     m_depthCubemapShaderNonSkeletal->SetFarPlane(std::sqrtf(pointLightPtr->GetRadianceSqrRadius()));
                     m_depthCubemapShaderNonSkeletal->SetPointLightPosition(pointLightPtr->GetPosition());

                     primitive->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
                  }
                  m_depthCubemapShaderNonSkeletal->StopShader();
               }
               if (shadowSkeletalMeshPrimitives.size() > 0) // Skeletal primitives
               {
                  m_depthCubemapShaderSkeletal->ExecuteShader();
                  for (auto& primitive : shadowSkeletalMeshPrimitives)
                  {
                     SkeletalMeshSceneProxy* skeletalProxy = static_cast<SkeletalMeshSceneProxy*>(primitive);

                     const auto& worldMatrix = skeletalProxy->GetMatrix();
                     const auto& viewMatrices = shadowInfo->GetShadowViewMatrices();
                     const auto& projectionMatrices = shadowInfo->GetShadowProjectionMatrices();

                     m_depthCubemapShaderSkeletal->SetTransformationMatrices(worldMatrix, viewMatrices, projectionMatrices);
                     m_depthCubemapShaderSkeletal->SetFarPlane(std::sqrtf(pointLightPtr->GetRadianceSqrRadius()));
                     m_depthCubemapShaderSkeletal->SetPointLightPosition(pointLightPtr->GetPosition());
                     m_depthCubemapShaderSkeletal->SetSkinningMatrices(skeletalProxy->GetSkinningMatrices());

                     skeletalProxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
                  }
                  m_depthCubemapShaderSkeletal->StopShader();
               }
               glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
         }
      }

      void DeferredShadingSceneRenderer::DeferredBasePass_RenderThread(std::vector<PrimitiveSceneProxy*>& nonSkeletalMeshPrimitives, std::vector<PrimitiveSceneProxy*>& skeletalMeshPrimitives, const glm::mat4& viewMatrix)
      {
         {
            // Deferred shading collect info
            m_gbuffer->BindDeferredGBuffer();

            if (skeletalMeshPrimitives.size() > 0)
            {
               //m_deferredBaseShaderSkeletal->ExecuteShader();
               mTestShaderSkelet->ExecuteShader();
               for (auto& proxy : skeletalMeshPrimitives)
               {
                  //m_deferredBaseShaderSkeletal->SetSkinningMatrices(skeletalProxy->GetSkinningMatrices());
                  //m_deferredBaseShaderSkeletal->SetTransformMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);
                  //proxy->GetAlbedo()->BindTexture(0);
                  //proxy->GetNormalMap()->BindTexture(1);
                  //m_deferredBaseShaderSkeletal->SetAlbedoTextureSlot(0);
                  //m_deferredBaseShaderSkeletal->SetNormalTextureSlot(1);
                  SkeletalMeshSceneProxy* skeletalProxy = static_cast<SkeletalMeshSceneProxy*>(proxy);
                  mTestShaderSkelet->GetVertexFactoryShader()->SetSkinningMatrices(skeletalProxy->GetSkinningMatrices());
                  const glm::mat4& worldMatrix = proxy->GetMatrix();
                  mTestShaderSkelet->GetVertexFactoryShader()->SetMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);
                  mTestShaderSkelet->SetMaterialShaderUniformValues(*mMatSkelet);
                  proxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
               }
               mTestShaderSkelet->StopShader();
               //m_deferredBaseShaderSkeletal->StopShader();
            }

            if (nonSkeletalMeshPrimitives.size() > 0)
            {
               //m_deferredBaseShaderNonSkeletal->ExecuteShader();
               mTestShaderStatic->ExecuteShader();
               for (auto& proxy : nonSkeletalMeshPrimitives)
               {
                  // m_deferredBaseShaderNonSkeletal->SetTransformMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);
                  //proxy->GetAlbedo()->BindTexture(0);
                  //proxy->GetNormalMap()->BindTexture(1);
                  //m_deferredBaseShaderNonSkeletal->SetAlbedoTextureSlot(0);
                  //m_deferredBaseShaderNonSkeletal->SetNormalTextureSlot(1);
                  const glm::mat4& worldMatrix = proxy->GetMatrix();
                  mTestShaderStatic->GetVertexFactoryShader()->SetMatrices(worldMatrix, viewMatrix, m_scene->ProjectionMatrix);
                  mTestShaderStatic->SetMaterialShaderUniformValues(*mMatStatic);
                  proxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
               }
               mTestShaderStatic->StopShader();
               //m_deferredBaseShaderNonSkeletal->StopShader();
              
            }

         m_gbuffer->UnbindDeferredGBuffer();
      }
   }

      void DeferredShadingSceneRenderer::DeferredLightPass_RenderThread(const std::vector<std::shared_ptr<LightSceneProxy>>& lightSourcesProxy)
      {
         {
            // TODO: Make some check if light source (point or spot light) is too far from current view position
            m_deferredLightShader->ExecuteShader();

            // ************************** SHADOWS ************************** //
            size_t pointLightIndex = 0, dirLightIndex = 0, shadowMapSlot = 3, dirShadowMapCount = 0, pointShadowMapCount = 0;
            for (auto& lightProxy : lightSourcesProxy)
            {
               if (lightProxy->GetLightProxyType() == LightSceneProxyType::DIR_LIGHT)
               {
                  DirectionalLightSceneProxy* lightPtr = static_cast<DirectionalLightSceneProxy*>(lightProxy.get());
                  ProjectedDirShadowInfo* shadowInfo = lightPtr->GetProjectedDirShadowInfo();
                  if (shadowInfo)
                  {
                     shadowInfo->GetAtlasResource()->BindTexture(shadowMapSlot);
                     m_deferredLightShader->SetDirectionalLightShadowMapSlot(dirLightIndex, shadowMapSlot, shadowInfo->GetPosOffsetShadowMapAtlas());
                     m_deferredLightShader->SetDirectionalLightShadowMatrix(dirLightIndex, shadowInfo->GetShadowMatrix());

                     dirShadowMapCount++;
                     dirLightIndex++;
                     shadowMapSlot++;
                  }
               }
               else if (lightProxy->GetLightProxyType() == LightSceneProxyType::POINT_LIGHT)
               {
                  PointLightSceneProxy* lightPtr = static_cast<PointLightSceneProxy*>(lightProxy.get());
                  ProjectedPointShadowInfo* shadowInfo = lightPtr->GetProjectedPointShadowInfo();
                  if (shadowInfo)
                  {
                     shadowInfo->GetAtlasResource()->BindTexture(shadowMapSlot);
                     m_deferredLightShader->SetPointLightShadowMapSlot(pointLightIndex, shadowMapSlot);
                     m_deferredLightShader->SetPointLightShadowProjectionFarPlane(pointLightIndex, std::sqrtf(lightPtr->GetRadianceSqrRadius()));
                     shadowMapSlot++;
                     pointShadowMapCount++;
                     pointLightIndex++;
                  }
               }
            }
            m_deferredLightShader->SetDirectionalLightShadowMapCount(dirShadowMapCount);
            m_deferredLightShader->SetPointLightShadowMapCount(pointShadowMapCount);
            // ************************** SHADOWS ************************** //

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

         /***** Access view matrix from GAME THREAD  *****/
         const glm::mat4 viewMatrix = m_scene->GetCamera()->GetViewMatrix();

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

         std::vector<PrimitiveSceneProxy*> skeletalPrimitives, nonSkeletalPrimitives;
         for (auto& proxy : drawDeferredShadedPrimitives)
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

         DepthPass(nonSkeletalPrimitives, skeletalPrimitives, m_scene->LightProxies);

         DeferredBasePass_RenderThread(nonSkeletalPrimitives, skeletalPrimitives, viewMatrix);

         DeferredLightPass_RenderThread(m_scene->LightProxies);

         if (bIsForwardShadedPrimitives)
         {
            ForwardBasePass_RenderThread(drawForwardShadedPrimitives, viewMatrix);
         }

         DebugFramePanelsPass();

      }

      void DeferredShadingSceneRenderer::DebugFramePanelsPass()
      {
         glDisable(GL_DEPTH_TEST);
         m_textureRenderer.RenderFrames(m_gbuffer);
      }

      void DeferredShadingSceneRenderer::PushRenderTargetToTextureRenderer()
      {
         m_textureRenderer.PushDebugRenderTarget();
      }
	}
}