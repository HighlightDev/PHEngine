#include "DepthShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      DepthShader<false>::DepthShader(std::string&& vsPath, std::string&& fsPath)
         : ShaderBase("Depth shader", std::move(vsPath), std::move(fsPath))
      {
         Init();
      }

      void DepthShader<false>::AccessAllUniformLocations() {

         ShaderBase::AccessAllUniformLocations();
         u_worldMatrix = GetUniform("worldMatrix");
         u_viewMatrix = GetUniform("shadowViewMatrix");
         u_projectionMatrix = GetUniform("shadowProjectionMatrix");
      }

      void DepthShader<false>::SetShaderPredefine() { }

      void DepthShader<false>::SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

      /*     SKELETAL MESH DEPTH SHADER   */

      void DepthShader<true>::AccessAllUniformLocations()
      {
         ShaderBase::AccessAllUniformLocations();
         u_boneMatrices = GetUniformArray("bonesMatrices", MaxBones);
         u_worldMatrix = GetUniform("worldMatrix");
         u_viewMatrix = GetUniform("shadowViewMatrix");
         u_projectionMatrix = GetUniform("shadowProjectionMatrix");
      }

      DepthShader<true>::DepthShader(std::string&& vsPath, std::string&& fsPath)
         : ShaderBase("Depth shader (Skeletal Mesh)", std::move(vsPath), std::move(fsPath))
      {
         Init();
      }

      void DepthShader<true>::SetShaderPredefine() 
      {
         Predefine<int32_t>(ShaderType::VertexShader, "MaxWeights", MaxWeights);
         Predefine<int32_t>(ShaderType::VertexShader, "MaxBones", MaxBones);

#undef MaxWeights
#undef MaxBones
      }

      void DepthShader<true>::SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

      void DepthShader<true>::SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices)
      {
         for (size_t index = 0; index < skinningMatrices.size(); index++)
            u_boneMatrices.LoadUniform(index, skinningMatrices[index]);
      }
   }
}