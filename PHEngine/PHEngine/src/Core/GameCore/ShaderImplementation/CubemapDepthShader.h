#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

#include <array>

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {

      class CubemapDepthShaderBase :
         public ShaderBase
      {
      public:

         using six_mat4x4 = std::array<glm::mat4x4, 6>;

      protected:

         Uniform u_worldMatrix;
         UniformArray u_shadowViewMatrices, u_shadowProjectionMatrices;

      protected:

         virtual void AccessAllUniformLocations() override;

      public:

         CubemapDepthShaderBase(const ShaderParams& params);

         void SetTransformationMatrices(const glm::mat4& worldMatrix, const six_mat4x4& viewMatrix, const six_mat4x4& projectionMatrix);

      };

      template <bool IsSkeletalMesh>
      class CubemapDepthShader;

      template <>
      class CubemapDepthShader<false>
         : public CubemapDepthShaderBase
      {
      public:

         CubemapDepthShader(const ShaderParams& params);

         virtual void SetShaderPredefine() override;
      };

      template <>
      class CubemapDepthShader<true>
         : public CubemapDepthShaderBase
      {
         UniformArray u_boneMatrices;

      public:

         CubemapDepthShader(const ShaderParams& params);

         void SetSkinningMatrices(const std::vector<glm::mat4>& skinningMatrices);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;
      };
   }
}

