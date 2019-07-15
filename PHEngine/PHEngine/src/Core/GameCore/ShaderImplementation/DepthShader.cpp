#include "DepthShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      DepthShaderBase::DepthShaderBase(const ShaderParams& params)
         : ShaderBase(params)
      {
      }

      void DepthShaderBase::AccessAllUniformLocations() {

         ShaderBase::AccessAllUniformLocations();
         u_worldMatrix = GetUniform("worldMatrix");
         u_shadowViewMatrix = GetUniform("shadowViewMatrix");
         u_shadowProjectionMatrix = GetUniform("shadowProjectionMatrix");
      }

      void DepthShaderBase::SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& shadowViewMatrix, const glm::mat4& shadowProjectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_shadowViewMatrix.LoadUniform(shadowViewMatrix);
         u_shadowProjectionMatrix.LoadUniform(shadowProjectionMatrix);
      }

      DepthShader<false>::DepthShader(const ShaderParams& params)
         : DepthShaderBase(params)
      {
         ShaderInit();
      }

      void DepthShader<false>::SetShaderPredefine() 
      {
      }

      /*     SKELETAL MESH DEPTH SHADER   */

#define MaxWeights 3
#define MaxBones 55

      DepthShader<true>::DepthShader(const ShaderParams& params)
         : DepthShaderBase(params)
      {
         ShaderInit();
      }

      void DepthShader<true>::AccessAllUniformLocations()
      {
         DepthShaderBase::AccessAllUniformLocations();
         u_boneMatrices = GetUniformArray("bonesMatrices", MaxBones);
      }

      void DepthShader<true>::SetShaderPredefine() 
      {
         DefineConstant<int32_t>(ShaderType::VertexShader, "MaxWeights", MaxWeights);
         DefineConstant<int32_t>(ShaderType::VertexShader, "MaxBones", MaxBones);
      }

#undef MaxWeights
#undef MaxBones

      void DepthShader<true>::SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices)
      {
         for (size_t index = 0; index < skinningMatrices.size(); index++)
            u_boneMatrices.LoadUniform(index, skinningMatrices[index]);
      }
   }
}