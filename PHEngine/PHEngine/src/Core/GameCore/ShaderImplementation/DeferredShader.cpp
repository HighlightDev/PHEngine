#include "DeferredShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      DeferredShader::DeferredShader(std::string&& vsPath, std::string&& fsPath)
         : ShaderBase("Deferred Shader", std::move(vsPath), std::move(fsPath))
      {
         Init();
      }

      DeferredShader::~DeferredShader()
      {

      }

      void DeferredShader::AccessAllUniformLocations()
      {
         u_worldMatrix = GetUniform("worldMatrix");
         u_viewMatrix = GetUniform("viewMatrix");
         u_projectionMatrix = GetUniform("projectionMatrix");
         u_albedoTex = GetUniform("albedo");
         u_normalTex = GetUniform("normal");
         u_specularTex = GetUniform("specular");
      }

      void DeferredShader::SetShaderPredefine()
      {
      }

      void DeferredShader::SetTransformMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

      void DeferredShader::SetAlbedoTextureSlot(int32_t slot) 
      {
         u_albedoTex.LoadUniform(slot);
      }

      void DeferredShader::SetNormalTextureSlot(int32_t slot)
      {
         u_normalTex.LoadUniform(slot);
      }

      void DeferredShader::SetSpecularTextureSlot(int32_t slot)
      {
         u_specularTex.LoadUniform(slot);
      }

   }
}