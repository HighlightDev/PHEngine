#include "CubemapRendererShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      CubemapRendererShader::CubemapRendererShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         Init();
      }

      CubemapRendererShader::~CubemapRendererShader()
      {
      }

      void CubemapRendererShader::SetCubemapSampler(int32_t cubemapSampler)
      {
         u_cubemapSampler.LoadUniform(cubemapSampler);
      }

      void CubemapRendererShader::SetWorldPositionGBuffeer(int32_t gBufferSampler)
      {
         u_worldPositionGBuffer.LoadUniform(gBufferSampler);
      }

      void CubemapRendererShader::SetScreenSpaceMatrix(const glm::mat4& screenSpaceMatrix)
      {
         u_screenSpaceMatrix.LoadUniform(screenSpaceMatrix);
      }

      void CubemapRendererShader::SetIsDepthTexture(bool bPerspectiveDepthTexture)
      {
         u_bPerspectiveDepthTexture.LoadUniform(bPerspectiveDepthTexture);
      }

      void CubemapRendererShader::SetProjectionFarPlane(float projectionFarPlane)
      {
         u_projectionFarPlane.LoadUniform(projectionFarPlane);
      }

      void CubemapRendererShader::SetPointLightPosition(glm::vec3 pointLightPosition)
      {
         u_pointLightPosition.LoadUniform(pointLightPosition);
      }

      void CubemapRendererShader::AccessAllUniformLocations()
      {
         Base::AccessAllUniformLocations();

         u_cubemapSampler = GetUniform("cubemapSampler");
         u_screenSpaceMatrix = GetUniform("screenSpaceMatrix");
         u_bPerspectiveDepthTexture = GetUniform("bPerspectiveDepthTexture");
         u_worldPositionGBuffer = GetUniform("worldPositionGBuffer");
         u_projectionFarPlane = GetUniform("projectionFarPlane");
         u_pointLightPosition = GetUniform("pointLightPosition");
      }

      void CubemapRendererShader::SetShaderPredefine()
      {
      }

   }
}
