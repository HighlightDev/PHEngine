#pragma once
#include "Shader.h"

namespace Graphics
{
   namespace OpenGL
   {

      template <typename ShaderType, typename MaterialShaderType, typename VertexFactoryShaderType>
      class CompositeShader
      {
      protected:

         using shader_t = ShaderType;
         using materialShader_t = MaterialShaderType;
         using vertexFactoryShader_t = VertexFactoryShaderType;

         shader_t mShader;
         materialShader_t mMaterialShader;
         vertexFactoryShader_t mVertexFactoryShader;

      public:

         template <typename UShader,
            typename UMaterialShader,
            typename UVertexFactoryShader>
         CompositeShader(UShader&& shader, UMaterialShader&& materialShader, UVertexFactoryShader&& vertexFactoryShader)
            : mShader(std::forward<UShader>(shader))
            , mMaterialShader(std::forward<UMaterialShader>(materialShader))
            , mVertexFactoryShader(std::forward<UVertexFactoryShader>(vertexFactoryShader))
         {
         }

         virtual ~CompositeShader()
         {

         }

         virtual void SetShaderPredefine()
         {

         }

         virtual void AccessAllUniformLocations()
         {
            for (size_t i = 0; i < std::tuple_size<typename materialShader_t::material_t::materialProperties_t>::value; ++i)
            {
               std::cout << mMaterialShader.mMaterial.mPropertiesName[i] << std::endl;
            }
         }

         void SetUniformValues()
         {
            mMaterialShader.SetUniformValues();
         }

         virtual void Init()
         {

         }
         
      };

   }
}

