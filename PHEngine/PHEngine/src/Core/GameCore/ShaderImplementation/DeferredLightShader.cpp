#include "DeferredLightShader.h"

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
         u_gBuffer_Position = GetUniform("gBuffer_Position");
         u_gBuffer_Normal = GetUniform("gBuffer_Normal");
         u_gBuffer_AlbedoNSpecular = GetUniform("gBuffer_AlbedoNSpecular");
      }

      void DeferredLightShader::SetShaderPredefine()
      {
         Predefine(FragmentShader, "DIR_LIGHT_COUNT", 1);
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

      void DeferredLightShader::SetDirLight(std::vector<DirectionalLight>& directionalLight)
      {
         for (size_t dirLightIndex = 0; dirLightIndex < directionalLight.size(); ++dirLightIndex)
         {
            u_DirLightAmbientColor.LoadUniform(dirLightIndex, directionalLight[dirLightIndex].Ambient);
            u_DirLightDiffuseColor.LoadUniform(dirLightIndex, directionalLight[dirLightIndex].Diffuse);
            u_DirLightSpecularColor.LoadUniform(dirLightIndex, directionalLight[dirLightIndex].Specular);
            u_DirLightDirection.LoadUniform(dirLightIndex, directionalLight[dirLightIndex].Direction);
         }
        
      }

   }
}