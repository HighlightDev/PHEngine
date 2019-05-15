#include "DeferredLightShader.h"
#include "Core/GraphicsCore/SceneProxy/DirectionalLightSceneProxy.h"
#include "Core/GraphicsCore/SceneProxy/PointLightSceneProxy.h"

namespace Game
{
   namespace ShaderImpl
   {

      DeferredLightShader::DeferredLightShader(std::string&& vsPath, std::string&& fsPath)
         : ShaderBase("Deferred Light Shader", std::move(vsPath), std::move(fsPath))
      {
         Init();
      }

      DeferredLightShader::~DeferredLightShader()
      {

      }

      void DeferredLightShader::AccessAllUniformLocations()
      {
         Base::AccessAllUniformLocations();

         u_DirLightAmbientColor = GetUniformArray("DirLightAmbientColor", DIR_LIGHT_COUNT);
         u_DirLightDiffuseColor = GetUniformArray("DirLightDiffuseColor", DIR_LIGHT_COUNT);
         u_DirLightSpecularColor = GetUniformArray("DirLightSpecularColor", DIR_LIGHT_COUNT);
         u_DirLightDirection = GetUniformArray("DirLightDirection", DIR_LIGHT_COUNT);

         u_PointLightDiffuseColor = GetUniformArray("PointLightDiffuseColor", POINT_LIGHT_COUNT);
         u_PointLightSpecularColor = GetUniformArray("PointLightSpecularColor", POINT_LIGHT_COUNT);
         u_PointLightPosition = GetUniformArray("PointLightPosition", POINT_LIGHT_COUNT);
         u_PointLightAttenuation = GetUniformArray("PointLightAttenuation", POINT_LIGHT_COUNT);

         u_gBuffer_Position = GetUniform("gBuffer_Position");
         u_gBuffer_Normal = GetUniform("gBuffer_Normal");
         u_gBuffer_AlbedoNSpecular = GetUniform("gBuffer_AlbedoNSpecular");

         u_DirectionalLightShadowMaps = GetUniformArray("DirLightShadowMaps", DIR_LIGHT_COUNT);
         u_DirectionalLightShadowMatrices = GetUniformArray("DirLightShadowMatrices", DIR_LIGHT_COUNT);
         u_DirectionalLightAtlasOffset = GetUniformArray("DirLightShadowAtlasOffset", DIR_LIGHT_COUNT);
         u_DirectionalLightShadowMapCount = GetUniform("DirLightShadowMapCount");
      }

      void DeferredLightShader::SetShaderPredefine()
      {
         Predefine(FragmentShader, "DIR_LIGHT_COUNT", DIR_LIGHT_COUNT);
         Predefine(FragmentShader, "POINT_LIGHT_COUNT", POINT_LIGHT_COUNT);
      }

      void DeferredLightShader::SetGBufferAlbedoNSpecular(int32_t slot)
      {
         u_gBuffer_AlbedoNSpecular.LoadUniform(slot);
      }

      void DeferredLightShader::SetGBufferNormal(int32_t slot)
      {
         u_gBuffer_Normal.LoadUniform(slot);
      }

      void DeferredLightShader::SetGBufferPosition(int32_t slot)
      {
         u_gBuffer_Position.LoadUniform(slot);
      }

      void DeferredLightShader::SetDirectionalLightShadowMapSlot(size_t index, int32_t slot, const glm::vec4& atlasOffset)
      {
         u_DirectionalLightShadowMaps.LoadUniform(index, slot);
         u_DirectionalLightAtlasOffset.LoadUniform(index, atlasOffset);
      }

      void DeferredLightShader::SetDirectionalLightShadowMapSlot(size_t index, int32_t slot)
      {
         u_DirectionalLightShadowMaps.LoadUniform(index, slot);
      }

      void DeferredLightShader::SetDirectionalLightShadowMapCount(int32_t count)
      {
         u_DirectionalLightShadowMapCount.LoadUniform(count);
      }

      void DeferredLightShader::SetDirectionalLightShadowMatrix(size_t index, const glm::mat4& shadowMatrix)
      {
         u_DirectionalLightShadowMatrices.LoadUniform(index, shadowMatrix);
      }

      void DeferredLightShader::SetLightsInfo(std::vector<std::shared_ptr<LightSceneProxy>> lightsProxies)
      {
         size_t dirLightProxyIndex = 0;
         for (size_t lightProxyIndex = 0; lightProxyIndex < lightsProxies.size(); ++lightProxyIndex)
         {
            if (lightsProxies[lightProxyIndex]->GetLightProxyType() == LightSceneProxyType::DIR_LIGHT && dirLightProxyIndex < DIR_LIGHT_COUNT)
            {
               auto sharedLightProxy = lightsProxies[lightProxyIndex];
               DirectionalLightSceneProxy* dirLProxyPtr = static_cast<DirectionalLightSceneProxy*>(sharedLightProxy.get());
               u_DirLightAmbientColor.LoadUniform(dirLightProxyIndex, dirLProxyPtr->AmbientColor);
               u_DirLightDiffuseColor.LoadUniform(dirLightProxyIndex, dirLProxyPtr->DiffuseColor);
               u_DirLightSpecularColor.LoadUniform(dirLightProxyIndex, dirLProxyPtr->SpecularColor);
               u_DirLightDirection.LoadUniform(dirLightProxyIndex, dirLProxyPtr->GetDirection());

               dirLightProxyIndex++;
            }
         }

         size_t pointLightProxyIndex = 0;
         for (size_t lightProxyIndex = 0; lightProxyIndex < lightsProxies.size(); ++lightProxyIndex)
         {
            if (lightsProxies[lightProxyIndex]->GetLightProxyType() == LightSceneProxyType::POINT_LIGHT && dirLightProxyIndex < POINT_LIGHT_COUNT)
            {
               auto sharedLightProxy = lightsProxies[lightProxyIndex];
               PointLightSceneProxy* pointLProxyPtr = static_cast<PointLightSceneProxy*>(sharedLightProxy.get());
               u_PointLightDiffuseColor.LoadUniform(pointLightProxyIndex, pointLProxyPtr->DiffuseColor);
               u_PointLightSpecularColor.LoadUniform(pointLightProxyIndex, pointLProxyPtr->SpecularColor);
               u_PointLightPosition.LoadUniform(pointLightProxyIndex, pointLProxyPtr->GetPosition());
               u_PointLightAttenuation.LoadUniform(pointLightProxyIndex, pointLProxyPtr->GetAttenuation());

               pointLightProxyIndex++;
            }
         }
      }

   }
}