#include "BillboardShader.h"


namespace Game
{
   namespace ShaderImpl
   {

      BillboardShader::BillboardShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         Init();
      }


      BillboardShader::~BillboardShader()
      {
      }

      void BillboardShader::SetTransformMatrices(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
      {
         u_worldMatrix.LoadUniform(worldMatrix);
         u_viewMatrix.LoadUniform(viewMatrix);
         u_projectionMatrix.LoadUniform(projectionMatrix);
      }

      void BillboardShader::SetTexture(int32_t texSlot)
      {
         u_texture.LoadUniform(texSlot);
      }

      void BillboardShader::AccessAllUniformLocations()
      {
         ShaderBase::AccessAllUniformLocations();

         u_worldMatrix = GetUniform("worldMatrix");
         u_viewMatrix = GetUniform("viewMatrix");
         u_projectionMatrix = GetUniform("projectionMatrix");
         u_texture = GetUniform("billboardTexture");
      }

      void BillboardShader::SetShaderPredefine()
      {
         DefineConstant<float>(ShaderType::GeometryShader, "billboardSize", 1.0f);
      }

   }
}