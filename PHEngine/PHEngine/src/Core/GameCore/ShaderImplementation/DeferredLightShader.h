#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"
#include "Core/GraphicsCore/SceneProxy/LightSceneProxy.h"

#include <string>

using namespace Graphics::OpenGL;
using namespace Graphics::Proxy;

#define MAX_DIR_LIGHT_COUNT 5
#define MAX_POINT_LIGHT_COUNT 1
#define SHADOWMAP_BIAS 0.005f
#define PCF_SAMPLES 2

namespace Game
{
   namespace ShaderImpl
   {

      class DeferredLightShader :
         public ShaderBase
      {

         using Base = ShaderBase;

         UniformArray u_DirLightAmbientColor;
         UniformArray u_DirLightDiffuseColor;
         UniformArray u_DirLightSpecularColor;
         UniformArray u_DirLightDirection;

         UniformArray u_DirectionalLightShadowMaps;
         UniformArray u_DirectionalLightShadowMatrices;
         UniformArray u_DirectionalLightAtlasOffset;
         Uniform u_DirectionalLightShadowMapCount;
         Uniform u_DirectionalLightCount;

         UniformArray u_PointLightDiffuseColor;
         UniformArray u_PointLightSpecularColor;
         UniformArray u_PointLightAttenuation;
         UniformArray u_PointLightPosition;

         UniformArray u_PointLightShadowMaps;
         UniformArray u_PointLightPositionWorld;
         Uniform u_PointLightShadowMapCount;
         Uniform u_PointLightCount;
         UniformArray u_PointLightShadowProjectionFarPlane;

         Uniform u_gBuffer_Position;
         Uniform u_gBuffer_Normal;
         Uniform u_gBuffer_AlbedoNSpecular;

      public:

         DeferredLightShader(const ShaderParams& params);

         virtual ~DeferredLightShader();

         void SetGBufferAlbedoNSpecular(int32_t slot);

         void SetGBufferNormal(int32_t slot);

         void SetGBufferPosition(int32_t slot);

         void SetLightsInfo(const std::vector<std::shared_ptr<LightSceneProxy>>& lightsProxies);

         void SetDirectionalLightShadowMapSlot(size_t index, int32_t slot);
         void SetDirectionalLightShadowMapSlot(size_t index, int32_t slot, const glm::vec4& atlasOffset);
         void SetDirectionalLightShadowMapCount(int32_t count);
         void SetDirectionalLightShadowMatrix(size_t index, const glm::mat4& shadowMatrix);

         void SetPointLightShadowMapSlot(size_t index, int32_t slot);
         void SetPointLightPosition(size_t index, const glm::vec3& position);
         void SetPointLightShadowMapCount(int32_t count);
         void SetPointLightShadowProjectionFarPlane(size_t index, float FarPlane);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

   }
}

