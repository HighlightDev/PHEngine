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
         u_specularTex = GetUniform("specular");
      }

      void DeferredShader::SetShaderPredefine()
      {

      }

   }
}