#pragma once
#include "Shader.h"
#include "Core/CommonApi/StringHash.h"

namespace Graphics
{
   namespace OpenGL
   {

      template <typename VertexFactoryShaderType, typename ShaderType, typename MaterialShaderType>
      class CompositeShader
         : public IShader
      {
      public:

         using vertexFactoryShader_t = VertexFactoryShaderType;
         using shader_t = ShaderType;
         using materialShader_t = MaterialShaderType;

      protected:
         std::shared_ptr<shader_t> mShader;
         std::unique_ptr<vertexFactoryShader_t> mVertexFactoryShader;
         std::unique_ptr<materialShader_t> mMaterialShader;

      public:

         template <typename UShader>
         CompositeShader(const std::string& compositeShaderName, const UShader& shader)
            : IShader(compositeShaderName)
            , mVertexFactoryShader(std::make_unique<vertexFactoryShader_t>())
            , mShader(std::static_pointer_cast<ShaderType>(shader))
            , mMaterialShader(std::make_unique<materialShader_t>())
         {
            Init();
         }

         virtual ~CompositeShader()
         {
         }

         void SetMaterialShaderUniformValues(typename materialShader_t::materialInstance_t& matInstance)
         {
            mMaterialShader->SetUniformValues(matInstance);
         }

         std::unique_ptr<VertexFactoryShaderType>& GetVertexFactoryShader()
         {
            return mVertexFactoryShader;
         }

         std::shared_ptr<ShaderType> GetShader() const
         {
            return mShader;
         }

         std::unique_ptr<MaterialShaderType>& GetMaterialShader()
         {
            return mMaterialShader;
         }

      private:

         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) override
         {
            IShader::AccessAllUniformLocations(shaderProgramID);

            mVertexFactoryShader->AccessAllUniformLocations(shaderProgramID);
            mShader->AccessAllUniformLocations(shaderProgramID);
            mMaterialShader->AccessAllUniformLocations(shaderProgramID);
         }

         virtual void ProcessAllPredefines() override
         {
            mVertexFactoryShader->ProcessAllPredefines();
            mShader->ProcessAllPredefines();
            mMaterialShader->ProcessAllPredefines();
         }

         bool AssembleShaderSource()
         {
            const std::string vertexFactoryShaderSource = mVertexFactoryShader->GetShaderSource();
            const std::string materialShaderSource = mMaterialShader->GetShaderSource();

            ShaderParams shaderParams = mShader->GetShaderParams();

            std::string vsSourcePath = EngineUtility::ConvertFromRelativeToAbsolutePath(shaderParams.VertexShaderFile);
            std::string fsSourcePath = EngineUtility::ConvertFromRelativeToAbsolutePath(shaderParams.FragmentShaderFile);
            std::string gsSourcePath = EngineUtility::ConvertFromRelativeToAbsolutePath(shaderParams.GeometryShaderFile);

            auto vsSource = LoadShaderSource(vsSourcePath);
            auto fsSource = LoadShaderSource(fsSourcePath);
            auto gsSource = LoadShaderSource(gsSourcePath);

            // Vertex Factory shader is combined with vertex shader
            vsSource = vertexFactoryShaderSource + "\n" + vsSource;

            // Material shader is combined with fragment shader
            fsSource = materialShaderSource + fsSource;

            ProcessShaderIncludes(vsSource);
            ProcessShaderIncludes(gsSource);
            ProcessShaderIncludes(fsSource);

            return SendToGpuShadersSources(vsSource, gsSource, fsSource);
         }

         void Init()
         {
            ProcessAllPredefines();

            const bool bShaderLoadedSuccessfully = AssembleShaderSource();
            if (bShaderLoadedSuccessfully)
            {
               CompileShaders();
               m_shaderProgramID = glCreateProgram();
               LinkShaders();
               AccessAllUniformLocations(m_shaderProgramID);
            }
         }
      };
   }
}

