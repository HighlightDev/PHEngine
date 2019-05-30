#include "DepthShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      DepthShader<false>::DepthShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         Init();
      }

      void DepthShader<false>::AccessAllUniformLocations() {

         ShaderBase::AccessAllUniformLocations();
         u_worldMatrix = GetUniform("worldMatrix");
         u_shadowViewMatrix = GetUniform("shadowViewMatrix");
         u_shadowProjectionMatrix = GetUniform("shadowProjectionMatrix");
      }

      void DepthShader<false>::SetShaderPredefine() { }

      void DepthShader<false>::SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& shadowViewMatrix, const glm::mat4& shadowProjectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_shadowViewMatrix.LoadUniform(shadowViewMatrix);
         u_shadowProjectionMatrix.LoadUniform(shadowProjectionMatrix);
      }

      /*     SKELETAL MESH DEPTH SHADER   */

#define MaxWeights 3
#define MaxBones 55

      void DepthShader<true>::AccessAllUniformLocations()
      {
         ShaderBase::AccessAllUniformLocations();
         u_boneMatrices = GetUniformArray("bonesMatrices", MaxBones);
         u_worldMatrix = GetUniform("worldMatrix");
         u_shadowViewMatrix = GetUniform("shadowViewMatrix");
         u_shadowProjectionMatrix = GetUniform("shadowProjectionMatrix");
      }

      DepthShader<true>::DepthShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         Init();
      }

      void DepthShader<true>::SetShaderPredefine() 
      {
         Predefine<int32_t>(ShaderType::VertexShader, "MaxWeights", MaxWeights);
         Predefine<int32_t>(ShaderType::VertexShader, "MaxBones", MaxBones);
      }

#undef MaxWeights
#undef MaxBones

      void DepthShader<true>::SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& shadowViewMatrix, const glm::mat4& shadowProjectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_shadowViewMatrix.LoadUniform(shadowViewMatrix);
         u_shadowProjectionMatrix.LoadUniform(shadowProjectionMatrix);
      }

      void DepthShader<true>::SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices)
      {
         for (size_t index = 0; index < skinningMatrices.size(); index++)
            u_boneMatrices.LoadUniform(index, skinningMatrices[index]);
      }
   }
}