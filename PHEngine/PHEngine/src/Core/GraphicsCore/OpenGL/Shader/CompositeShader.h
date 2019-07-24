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
            mVertexFactoryShader.AccessAllUniformLocations(shaderProgramID);
            mShader.AccessAllUniformLocations(shaderProgramID);
            mMaterialShader.AccessAllUniformLocations(shaderProgramID);
         }

         virtual void ProcessAllPredefines() override
         {
            mVertexFactoryShader.ProcessAllPredefines();
            mShader.ProcessAllPredefines();
            mMaterialShader.ProcessAllPredefines();
         }

         void SetUniformValues()
         {
            mMaterialShader.SetUniformValues();
         }

         bool AssembleShaderSource()
         {
            const std::string vertexFactoryShaderSource = mVertexFactoryShader.GetShaderSource();
            const std::string materialShaderSource = mMaterialShader.GetShaderSource();

            ShaderParams shaderParams = mShader.GetShaderParams();

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

            m_shaderCompiledSuccessfully = false;
            const bool bShaderLoadedSuccessfully = AssembleShaderSource();
            if (bShaderLoadedSuccessfully)
            {
               m_shaderCompiledSuccessfully = CompileShaders();
            }
            if (m_shaderCompiledSuccessfully)
            {
               m_shaderProgramID = glCreateProgram();
               const bool bLinkedSuccessfully = LinkShaders();
               if (bLinkedSuccessfully)
               {
                  AccessAllUniformLocations(m_shaderProgramID);
               }
            }
         }
      };

   }
}

