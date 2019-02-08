#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"
#include "Core/GraphicsCore/Light/DirectionalLight.h"
#include "Core/GraphicsCore/SceneProxy/LightSceneProxy.h"

#include <string>

using namespace Graphics::OpenGL;
using namespace Graphics::Light;
using namespace Graphics::Proxy;

#define DIR_LIGHT_COUNT 1

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

         Uniform u_gBuffer_Position;
         Uniform u_gBuffer_Normal;
         Uniform u_gBuffer_AlbedoNSpecular;
      public:

         DeferredLightShader(std::string&& vsPath, std::string&& fsPath);

         virtual ~DeferredLightShader();

         void SetGBufferAlbedoNSpecular(int32_t slot);

         void SetGBufferNormal(int32_t slot);

         void SetGBufferPosition(int32_t slot);

         void SetDirLight(std::vector<DirectionalLight>& directionalLight);

         void SetLightsInfo(std::vector<std::shared_ptr<LightSceneProxy>> lightsProxies);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

   }
}

