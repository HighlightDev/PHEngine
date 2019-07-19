#pragma once
#include "Shader.h"

namespace Graphics
{
   namespace OpenGL
   {

      template <typename VertexFactoryShaderType, typename ShaderType, typename MaterialShaderType>
      class CompositeShader
      {
      protected:

         using vertexFactoryShader_t = VertexFactoryShaderType;
         using shader_t = ShaderType;
         using materialShader_t = MaterialShaderType;

         shader_t mShader;
         materialShader_t mMaterialShader;
         vertexFactoryShader_t mVertexFactoryShader;

      public:

         template <typename UShader, typename UMaterialShader, typename UVertexFactoryShader>
         CompositeShader(UShader&& shader, UMaterialShader&& materialShader, UVertexFactoryShader&& vertexFactoryShader)
            : mShader(std::forward<UShader>(shader))
            , mMaterialShader(std::forward<UMaterialShader>(materialShader))
            , mVertexFactoryShader(std::forward<UVertexFactoryShader>(vertexFactoryShader))
         {
         }

         virtual ~CompositeShader()
         {

         }

         void AccessAllUniformLocations()
         {
            uint32_t mProgramId = 0; // TODO: temporary

            //mShader.AccessAllUniformLocations(mProgramId);
            mMaterialShader.AccessAllUniformLocations(mProgramId);
         }

         void SetShaderPredefine()
         {

         }

         virtual void SetUniformValues()
         {
            mMaterialShader.SetUniformValues();
         }

         virtual void Init()
         {

         }
         
      };

   }
}

