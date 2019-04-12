#include "DeferredShader.h"

//namespace Game
//{
//   namespace ShaderImpl
//   {
//
//      DeferredShader::DeferredShader(std::string&& vsPath, std::string&& fsPath)
//         : ShaderBase("Deferred Shader", std::move(vsPath), std::move(fsPath))
//      {
//         Init();
//      }
//
//      DeferredShader::~DeferredShader()
//      {
//
//      }
//
//      void DeferredShader::AccessAllUniformLocations()
//      {
//         Base::AccessAllUniformLocations();
//
//         u_isSkeletal = GetUniform("isSkeletal");
//         u_boneMatrices = GetUniformArray("bonesMatrices", MaxBones);
//
//         u_worldMatrix = GetUniform("worldMatrix");
//         u_viewMatrix = GetUniform("viewMatrix");
//         u_projectionMatrix = GetUniform("projectionMatrix");
//         u_albedoTex = GetUniform("albedo");
//         u_normalTex = GetUniform("normalMap");
//         u_specularTex = GetUniform("specularMap");
//      }
//
//      void DeferredShader::SetShaderPredefine()
//      {
//         int32_t maxWeights = MaxWeights;
//         int32_t maxBones = MaxBones;
//         Predefine<int32_t>(ShaderType::VertexShader, "MaxWeights", std::move(maxWeights));
//         Predefine<int32_t>(ShaderType::VertexShader, "MaxBones", std::move(maxBones));
//      }
//
//      void DeferredShader::SetTransformMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
//      {
//         u_worldMatrix.LoadUniform(worldMatrix);
//         u_viewMatrix.LoadUniform(viewMatrix);
//         u_projectionMatrix.LoadUniform(projectionMatrix);
//      }
//
//      void DeferredShader::SetAlbedoTextureSlot(int32_t slot) 
//      {
//         u_albedoTex.LoadUniform(slot);
//      }
//
//      void DeferredShader::SetNormalTextureSlot(int32_t slot)
//      {
//         u_normalTex.LoadUniform(slot);
//      }
//
//      void DeferredShader::SetSpecularTextureSlot(int32_t slot)
//      {
//         u_specularTex.LoadUniform(slot);
//      }
//
//      void DeferredShader::SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices)
//      {
//         for (size_t index = 0; index < skinningMatrices.size(); index++)
//            u_boneMatrices.LoadUniform(index, skinningMatrices[index]);
//
//         u_isSkeletal.LoadUniform(true);
//      }
//
//      void DeferredShader::SetNotSkeletalMesh()
//      {
//         u_isSkeletal.LoadUniform(false);
//      }
//
//   }
//}