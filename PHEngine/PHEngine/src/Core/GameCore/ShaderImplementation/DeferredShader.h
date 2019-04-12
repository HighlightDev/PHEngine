#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

#include <string>

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {
    
      template <bool SkeletalMeshShader>
      class DeferredShader;

      template <> 
      class DeferredShader<false> 
         : public ShaderBase
      {
         using Base = ShaderBase;

         Uniform u_worldMatrix, u_viewMatrix, u_projectionMatrix, u_albedoTex, u_normalTex, u_specularTex;

      public:

         DeferredShader(std::string&& vsPath, std::string&& fsPath)
            : ShaderBase("Deferred Shader (Non Skeletal Meshes)", std::move(vsPath), std::move(fsPath))
         {
            Init();
         }

         void SetTransformMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
         {
            u_worldMatrix.LoadUniform(worldMatrix);
            u_viewMatrix.LoadUniform(viewMatrix);
            u_projectionMatrix.LoadUniform(projectionMatrix);
         }

         void SetAlbedoTextureSlot(int32_t slot)
         {
            u_albedoTex.LoadUniform(slot);
         }

         void SetNormalTextureSlot(int32_t slot)
         {
            u_normalTex.LoadUniform(slot);
         }

         void SetSpecularTextureSlot(int32_t slot)
         {
            u_specularTex.LoadUniform(slot);
         }

      protected:

         void AccessAllUniformLocations()
         {
            Base::AccessAllUniformLocations();

            u_worldMatrix = GetUniform("worldMatrix");
            u_viewMatrix = GetUniform("viewMatrix");
            u_projectionMatrix = GetUniform("projectionMatrix");
            u_albedoTex = GetUniform("albedo");
            u_normalTex = GetUniform("normalMap");
            u_specularTex = GetUniform("specularMap");
         }

         void SetShaderPredefine()
         {
         }

      };

      template <>
      class DeferredShader<true> :
         public ShaderBase
      {

         using Base = ShaderBase;

         const int32_t MaxWeights = 3;
         const int32_t MaxBones = 55;

         Uniform u_worldMatrix, u_viewMatrix, u_projectionMatrix, u_albedoTex, u_normalTex, u_specularTex;
         UniformArray u_boneMatrices;

      public:

         DeferredShader(std::string&& vsPath, std::string&& fsPath)
            : ShaderBase("Deferred Shader (Skeletal Mesh)", std::move(vsPath), std::move(fsPath))
         {
            Init();
         }

         void SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices)
         {
            for (size_t index = 0; index < skinningMatrices.size(); index++)
               u_boneMatrices.LoadUniform(index, skinningMatrices[index]);
         }

         void SetTransformMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
         {
            u_worldMatrix.LoadUniform(worldMatrix);
            u_viewMatrix.LoadUniform(viewMatrix);
            u_projectionMatrix.LoadUniform(projectionMatrix);
         }

         void SetAlbedoTextureSlot(int32_t slot)
         {
            u_albedoTex.LoadUniform(slot);
         }

         void SetNormalTextureSlot(int32_t slot)
         {
            u_normalTex.LoadUniform(slot);
         }

         void SetSpecularTextureSlot(int32_t slot)
         {
            u_specularTex.LoadUniform(slot);
         }

         void AccessAllUniformLocations()
         {
            Base::AccessAllUniformLocations();

            u_boneMatrices = GetUniformArray("bonesMatrices", MaxBones);

            u_worldMatrix = GetUniform("worldMatrix");
            u_viewMatrix = GetUniform("viewMatrix");
            u_projectionMatrix = GetUniform("projectionMatrix");
            u_albedoTex = GetUniform("albedo");
            u_normalTex = GetUniform("normalMap");
            u_specularTex = GetUniform("specularMap");
         }

         void SetShaderPredefine()
         {
            int32_t maxWeights = MaxWeights;
            int32_t maxBones = MaxBones;
            Predefine<int32_t>(ShaderType::VertexShader, "MaxWeights", std::move(maxWeights));
            Predefine<int32_t>(ShaderType::VertexShader, "MaxBones", std::move(maxBones));
         }

      };

   }
}

