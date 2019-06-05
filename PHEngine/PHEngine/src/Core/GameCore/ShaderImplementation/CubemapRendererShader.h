#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {
      class CubemapRendererShader :
         public ShaderBase
      {
         using Base = ShaderBase;

         Uniform u_cubemapSampler
            , u_worldPositionGBuffer
            , u_screenSpaceMatrix
            , u_bPerspectiveDepthTexture
            , u_projectionFarPlane
            , u_pointLightPosition;

      public:

         CubemapRendererShader(const ShaderParams& params);

         virtual ~CubemapRendererShader();

         void SetCubemapSampler(int32_t cubemapSampler);

         void SetScreenSpaceMatrix(const glm::mat4& screenSpaceMatrix);

         void SetIsDepthTexture(bool bPerspectiveDepthTexture);

         void SetWorldPositionGBuffeer(int32_t gBufferSampler);

         void SetProjectionFarPlane(float projectionFarPlane);

         void SetPointLightPosition(glm::vec3 pointLightPosition);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;
      };
   }
}

