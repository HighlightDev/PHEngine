#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

#include <string>

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {

      template <bool IsSkeletalMesh>
      class DepthShader;

      template <>
      class DepthShader<false> :
         public ShaderBase
      {
         using Base = ShaderBase;

         Uniform u_worldMatrix, u_viewMatrix, u_projectionMatrix;

      public:

         DepthShader(std::string&& vsPath, std::string&& fsPath);

         void SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

#define MaxWeights 3
#define MaxBones 55

      template <>
      class DepthShader<true> :
         public ShaderBase
      {
         using Base = ShaderBase;

         Uniform u_worldMatrix, u_viewMatrix, u_projectionMatrix;
         UniformArray u_boneMatrices;

      public:

         DepthShader(std::string&& vsPath, std::string&& fsPath);

         void SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

         void SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

   }
}

