#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

#include <string>

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {

      class DeferredShader :
         public ShaderBase
      {

         using Base = ShaderBase;

         Uniform u_worldMatrix, u_viewMatrix, u_projectionMatrix, u_albedoTex, u_normalTex, u_specularTex;

      public:

         DeferredShader(std::string&& vsPath, std::string&& fsPath);

         virtual ~DeferredShader();

         void SetTransformMatrices(const glm::mat4& worldMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

         void SetAlbedoTextureSlot(int32_t slot);

         void SetNormalTextureSlot(int32_t slot);

         void SetSpecularTextureSlot(int32_t slot);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

   }
}

