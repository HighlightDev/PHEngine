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

         Uniform u_worldMatrix, u_viewMatrix, u_projectionMatrix, u_albedoTex, u_specularTex;

      public:

         DeferredShader(std::string&& vsPath, std::string&& fsPath);

         virtual ~DeferredShader();

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;

      };

   }
}

