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

      void CubemapDepthShaderBase::SetTransformationMatrices(const glm::mat4& worldMatrix, const mat4x4tuple_t& viewMatrix, const mat4x4tuple_t& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_shadowViewMatrices.LoadUniform(0, std::get<0>(viewMatrix));
         u_shadowViewMatrices.LoadUniform(1, std::get<1>(viewMatrix));
         u_shadowViewMatrices.LoadUniform(2, std::get<2>(viewMatrix));
         u_shadowViewMatrices.LoadUniform(3, std::get<3>(viewMatrix));
         u_shadowViewMatrices.LoadUniform(4, std::get<4>(viewMatrix));
         u_shadowViewMatrices.LoadUniform(5, std::get<5>(viewMatrix));

         u_shadowProjectionMatrices.LoadUniform(0, std::get<0>(projectionMatrix));
         u_shadowProjectionMatrices.LoadUniform(1, std::get<1>(projectionMatrix));
         u_shadowProjectionMatrices.LoadUniform(2, std::get<2>(projectionMatrix));
         u_shadowProjectionMatrices.LoadUniform(3, std::get<3>(projectionMatrix));
         u_shadowProjectionMatrices.LoadUniform(4, std::get<4>(projectionMatrix));
         u_shadowProjectionMatrices.LoadUniform(5, std::get<5>(projectionMatrix));
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
         Predefine<int32_t>(ShaderType::GeometryShader, "CubemapSides", 6);
      }

#undef MaxWeights
#undef MaxBones

   }
}
