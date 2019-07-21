#pragma once
#include "Shader.h"

namespace Graphics
{
   namespace OpenGL
   {

      template <typename VertexFactoryShaderType, typename ShaderType, typename MaterialShaderType>
      class CompositeShader
         : public IShader
      {
      protected:

         using vertexFactoryShader_t = VertexFactoryShaderType;
         using shader_t = ShaderType;
         using materialShader_t = MaterialShaderType;

         shader_t mShader;
         vertexFactoryShader_t mVertexFactoryShader;
         materialShader_t mMaterialShader;

      public:

         template <typename UVertexFactoryShader, typename UShader, typename UMaterialShader>
         CompositeShader(const std::string& compositeShaderName, UVertexFactoryShader&& vertexFactoryShader, UShader&& shader, UMaterialShader&& materialShader)
            : IShader(compositeShaderName)
            , mVertexFactoryShader(std::forward<UVertexFactoryShader>(vertexFactoryShader))
            , mShader(std::forward<UShader>(shader))
            , mMaterialShader(std::forward<UMaterialShader>(materialShader))
         {
            Init();
         }

         virtual ~CompositeShader()
         {

         }

         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) override
         {

            //mShader.AccessAllUniformLocations(mProgramId);
            mMaterialShader.AccessAllUniformLocations(shaderProgramID);
         }

         virtual void SetShaderPredefine() override
         {

         }

         virtual void ProcessAllPredefines() override
         {

         }

         void SetUniformValues()
         {
            mMaterialShader.SetUniformValues();
         }

         void Init()
         {
            AccessAllUniformLocations(m_shaderProgramID);
         }
         
      };

   }
}

