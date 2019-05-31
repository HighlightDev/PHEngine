#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

#include <tuple>

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {

      class CubemapDepthShaderBase :
         public ShaderBase
      {
      public:

         using mat4x4tuple_t = std::tuple<glm::mat4, glm::mat4, glm::mat4, glm::mat4, glm::mat4, glm::mat4>;

      protected:

         Uniform u_worldMatrix;
         UniformArray u_shadowViewMatrices, u_shadowProjectionMatrices;

      protected:

         virtual void AccessAllUniformLocations() override;

      public:

         CubemapDepthShaderBase(const ShaderParams& params);

         void SetTransformationMatrices(const glm::mat4& worldMatrix, const mat4x4tuple_t& viewMatrix, const mat4x4tuple_t& projectionMatrix);

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

