#pragma once

#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {

      class CubemapShader : public ShaderBase
      {

      private:
         Uniform u_worldMatrix, u_viewMatrix, u_projectionMatrix, u_texture;

      public:
         CubemapShader(const ShaderParams& params);

         virtual ~CubemapShader();

         void SetTransformMatrices(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);

         void SetTexture(int32_t texSlot);

      protected:

         virtual void AccessAllUniformLocations() override;

         virtual void SetShaderPredefine() override;
      };

   }
}

