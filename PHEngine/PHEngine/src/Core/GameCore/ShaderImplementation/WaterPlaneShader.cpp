#include "WaterPlaneShader.h"

namespace Game
{
   namespace ShaderImpl
   {

      WaterPlaneShader::WaterPlaneShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         ShaderInit();
      }

      WaterPlaneShader::~WaterPlaneShader()
      {
      }

      void WaterPlaneShader::AccessAllUniformLocations()
      {
         u_bEnableSun = GetUniform("bEnableSun");
         u_modelMatrix = GetUniform("modelMatrix");
         u_viewMatrix = GetUniform("viewMatrix");
         u_projectionMatrix = GetUniform("projectionMatrix");
         u_reflectionTexture = GetUniform("reflectionTexture");
         u_refractionTexture = GetUniform("refractionTexture");
         u_cameraPosition = GetUniform("cameraPosition");
         u_dudvTexture = GetUniform("dudvTexture");
         u_normalMap = GetUniform("normalMap");
         u_depthTexture = GetUniform("depthTexture");
         u_moveFactor = GetUniform("moveFactor");
         u_sunPos = GetUniform("sunPos");
         u_sunSpecularColour = GetUniform("sunSpecularColour");
         u_waveStrength = GetUniform("waveStrength");
         u_farClipPlane = GetUniform("farClipPlane");
         u_nearClipPlane = GetUniform("nearClipPlane");
         u_transparencyDepth = GetUniform("transparencyDepth");

         u_mistEnable = GetUniform("mistEnable");
         u_mistDensity = GetUniform("mistDensity");
         u_mistGradient = GetUniform("mistGradient");
         u_mistColour = GetUniform("mistColour");
      }

      void WaterPlaneShader::SetShaderPredefine()
      {
         DefineConstant<int32_t>(ShaderType(VertexShader | FragmentShader), "MAX_LIGHT_COUNT", 1);
         DefineConstant<float>(ShaderType(VertexShader), "tiling", 15.0f);
         DefineConstant<float>(ShaderType(FragmentShader), "materialReflectivity", 0.7f);
         DefineConstant<float>(ShaderType(FragmentShader), "materialShineDamper", 100.0f);
      }

      void WaterPlaneShader::SetTransformationMatrices(glm::mat4& modelMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         u_modelMatrix.LoadUniform(modelMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

      void WaterPlaneShader::SetReflectionSampler(int32_t reflectionSampler)
      {
         u_reflectionTexture.LoadUniform(reflectionSampler);
      }

      void WaterPlaneShader::SetRefractionSampler(int32_t refractionSampler)
      {
         u_refractionTexture.LoadUniform(refractionSampler);
      }

      void WaterPlaneShader::SetDepthSampler(int32_t depthSampler)
      {
         u_depthTexture.LoadUniform(depthSampler);
      }

      void WaterPlaneShader::SetDuDvSampler(int32_t DuDvSampler)
      {
         u_dudvTexture.LoadUniform(DuDvSampler);
      }

      void WaterPlaneShader::SetNormalMapSampler(int32_t normalMapSampler)
      {
         u_normalMap.LoadUniform(normalMapSampler);
      }

      void WaterPlaneShader::SetCameraPosition(glm::vec3& cameraPosition)
      {
         u_cameraPosition.LoadUniform(cameraPosition);
      }

      void WaterPlaneShader::SetDistortionProperties(float moveFactor, float waveStrength)
      {
         u_moveFactor.LoadUniform(moveFactor);
         u_waveStrength.LoadUniform(waveStrength);
      }

      void WaterPlaneShader::SetClippingPlanes(float perspectiveClipPlaneNear, float perspectiveClipPlaneFar)
      {
         u_nearClipPlane.LoadUniform(perspectiveClipPlaneNear);
         u_farClipPlane.LoadUniform(perspectiveClipPlaneFar);
      }

      void WaterPlaneShader::SetTransparancyDepth(float transparencyDepth)
      {
         u_transparencyDepth.LoadUniform(transparencyDepth);
      }

   }
}
