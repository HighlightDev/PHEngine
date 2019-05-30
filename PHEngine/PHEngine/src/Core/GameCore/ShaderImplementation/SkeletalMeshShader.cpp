#include "SkeletalMeshShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      SkeletalMeshShader::SkeletalMeshShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         Init();
      }

      SkeletalMeshShader::~SkeletalMeshShader()
      {

      }

      void SkeletalMeshShader::SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

      void SkeletalMeshShader::SetAlbedoTex(int32_t slot)
      {
         u_albedo.LoadUniform(slot);
      }

      void SkeletalMeshShader::SetNormalMapTex(int32_t slot)
      {

      }

      void SkeletalMeshShader::SetSpecularMapTex(int32_t slot)
      {

      }

      void SkeletalMeshShader::AccessAllUniformLocations()
      {
         u_worldMatrix = GetUniform("worldMatrix");
         u_viewMatrix = GetUniform("viewMatrix");
         u_projectionMatrix = GetUniform("projectionMatrix");
         u_albedo = GetUniform("albedoTexture");
         u_boneMatrices = GetUniformArray("bonesMatrices", MAX_BONES);
      }

      void SkeletalMeshShader::SetSkinningMatrices(const std::vector<glm::mat4>& matrices)
      {
         for (size_t i = 0; i < matrices.size(); i++)
         {
            u_boneMatrices.LoadUniform(i, matrices[i]);
         }
      }

      void SkeletalMeshShader::SetShaderPredefine()
      {
         Predefine<int32_t>(ShaderType::VertexShader, "MaxWeights", MAX_WEIGHTS);
         Predefine<int32_t>(ShaderType::VertexShader, "MaxBones", MAX_BONES);
      }

   }
}