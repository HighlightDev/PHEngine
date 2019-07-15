#include "TextureRendererShader.h"


namespace Game
{
   namespace ShaderImpl
   {

      TextureRendererShader::TextureRendererShader(const ShaderParams& params)
         : ShaderBase(params)
      {
         Base::ShaderInit();
      }

      void TextureRendererShader::AccessAllUniformLocations() {

         Base::AccessAllUniformLocations();

         u_uiTexture = GetUniform("uiTexture");
         u_screenSpaceMatrix = GetUniform("screenSpaceMatrix");
         u_bPerspectiveDepthTexture = GetUniform("bPerspectiveDepthTexture");
         u_bSeparated = GetUniform("bSeparated");
      }

      void TextureRendererShader::SetUiTextureSampler(int32_t uiTextureSampler)
      {
         u_uiTexture.LoadUniform(uiTextureSampler);
      }

      void TextureRendererShader::SetScreenSpaceMatrix(const glm::mat4& screenSpaceMatrix)
      {
         u_screenSpaceMatrix.LoadUniform(screenSpaceMatrix);
      }

      void TextureRendererShader::SetIsDepthTexture(bool bPerspectiveDepthTexture)
      {
         u_bPerspectiveDepthTexture.LoadUniform(bPerspectiveDepthTexture);
      }

      void TextureRendererShader::SetIsSeparatedScreen(bool bSeparatedScreen)
      {
         u_bSeparated.LoadUniform(bSeparatedScreen);
      }

      void TextureRendererShader::SetShaderPredefine() {

         DefineConstant<float>(ShaderType::FragmentShader, "zNearPlane", 1.0f);
         DefineConstant<float>(ShaderType::FragmentShader, "zFarPlane", 100.0f);
      }

   }
}
