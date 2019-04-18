#include "DepthShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      DepthShader::DepthShader(std::string&& vsPath, std::string&& fsPath)
         : ShaderBase("Depth shader", std::move(vsPath), std::move(fsPath))
      {
         Init();
      }

      void DepthShader::AccessAllUniformLocations() {

         u_worldMatrix = GetUniform("worldMatrix");
         u_viewMatrix = GetUniform("viewMatrix");
         u_projectionMatrix = GetUniform("projectionMatrix");
      }

      void DepthShader::SetShaderPredefine() { }

      void DepthShader::SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

   }
}