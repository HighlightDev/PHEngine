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

         Uniform u_worldMatrix, u_shadowViewMatrix, u_shadowProjectionMatrix;

      public:

         DepthShader(const ShaderParams& params);

         void SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

      template <>
      class DepthShader<true> :
         public ShaderBase
      {
         using Base = ShaderBase;

         Uniform u_worldMatrix, u_shadowViewMatrix, u_shadowProjectionMatrix;
         UniformArray u_boneMatrices;

      public:

         DepthShader(const ShaderParams& params);

         void SetTransformationMatrices(const glm::mat4& worldMatrix, const glm::mat4& shadowViewMatrix, const glm::mat4& shadowProjectionMatrix);

         void SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

   }
}

