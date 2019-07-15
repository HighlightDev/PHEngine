#include "CubemapShader.h"


namespace Game
{
   namespace ShaderImpl
   {

      CubemapShader::CubemapShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         ShaderInit();
      }


      CubemapShader::~CubemapShader()
      {
      }

      void CubemapShader::SetTransformMatrices(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

      void CubemapShader::SetTexture(int32_t texSlot)
      {
         u_texture.LoadUniform(texSlot);
      }

      void CubemapShader::AccessAllUniformLocations()
      {
         ShaderBase::AccessAllUniformLocations();

         u_worldMatrix = GetUniform("worldMatrix");
         u_viewMatrix = GetUniform("viewMatrix");
         u_projectionMatrix = GetUniform("projectionMatrix");
         u_texture = GetUniform("cubeTexture");
      }

      void CubemapShader::SetShaderPredefine()
      {
      }

   }
}