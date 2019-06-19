#include "CubemapDepthShader.h"


namespace Game
{
   namespace ShaderImpl
   {
      CubemapDepthShaderBase::CubemapDepthShaderBase(const ShaderParams& params)
         : ShaderBase(params)
      {
      }

      void CubemapDepthShaderBase::AccessAllUniformLocations()
      {
         ShaderBase::AccessAllUniformLocations();
         u_worldMatrix = GetUniform("worldMatrix");
         u_shadowViewMatrices = GetUniformArray("shadowViewMatrices", 6);
         u_shadowProjectionMatrices = GetUniformArray("shadowProjectionMatrices", 6);
      }

      void CubemapDepthShaderBase::SetTransformationMatrices(const glm::mat4& worldMatrix, const six_mat4x4& viewMatrix, const six_mat4x4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_shadowViewMatrices.LoadUniform(0, viewMatrix[0]);
         u_shadowViewMatrices.LoadUniform(1, viewMatrix[1]);
         u_shadowViewMatrices.LoadUniform(2, viewMatrix[2]);
         u_shadowViewMatrices.LoadUniform(3, viewMatrix[3]);
         u_shadowViewMatrices.LoadUniform(4, viewMatrix[4]);
         u_shadowViewMatrices.LoadUniform(5, viewMatrix[5]);

         u_shadowProjectionMatrices.LoadUniform(0, projectionMatrix[0]);
         u_shadowProjectionMatrices.LoadUniform(1, projectionMatrix[1]);
         u_shadowProjectionMatrices.LoadUniform(2, projectionMatrix[2]);
         u_shadowProjectionMatrices.LoadUniform(3, projectionMatrix[3]);
         u_shadowProjectionMatrices.LoadUniform(4, projectionMatrix[4]);
         u_shadowProjectionMatrices.LoadUniform(5, projectionMatrix[5]);
      }

      // *************************  CubemapDepthShader (Non Skeletal)  *************************  //
      CubemapDepthShader<false>::CubemapDepthShader(const ShaderParams& params)
         : CubemapDepthShaderBase(params)
      {
         Init();
      }

      void CubemapDepthShader<false>::SetShaderPredefine() 
      {
         Predefine<int32_t>(ShaderType::GeometryShader, "CubemapFaces", 6);
      }

      // *************************  CubemapDepthShader (Skeletal)  *************************  //
      CubemapDepthShader<true>::CubemapDepthShader(const ShaderParams& params)
         : CubemapDepthShaderBase(params)
      {
         Init();
      }

      void CubemapDepthShader<true>::SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices)
      {
         for (size_t index = 0; index < skinningMatrices.size(); index++)
            u_boneMatrices.LoadUniform(index, skinningMatrices[index]);
      }

#define MaxWeights 3
#define MaxBones 55

      void CubemapDepthShader<true>::AccessAllUniformLocations()
      {
         CubemapDepthShaderBase::AccessAllUniformLocations();
         u_boneMatrices = GetUniformArray("bonesMatrices", MaxBones);
      }

      void CubemapDepthShader<true>::SetShaderPredefine()
      {
         Predefine<int32_t>(ShaderType::VertexShader, "MaxWeights", MaxWeights);
         Predefine<int32_t>(ShaderType::VertexShader, "MaxBones", MaxBones);
         Predefine<int32_t>(ShaderType::GeometryShader, "CubemapFaces", 6);
      }

#undef MaxWeights
#undef MaxBones

   }
}
