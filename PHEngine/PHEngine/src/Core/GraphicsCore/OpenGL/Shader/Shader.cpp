#include "Shader.h"

#include <glm/vec2.hpp>
#include <gl/glew.h>
#include <fstream>

#if DEBUG
#include <iostream>
#endif

namespace Graphics
{
	namespace OpenGL
	{

#if DEBUG
      bool Shader::bParentAccessUniformLocationsInvoked = false;
#endif

      Shader::Shader(const ShaderParams& params)
         : IShader(params.ShaderName)
         , m_shaderParams(params)
         , m_defineConstantParameters()
         , m_defines()
      {
      }

		Shader::~Shader()
		{
		}

		void Shader::ShaderInit()
		{
			SetShaderPredefine(); // start precompile shader customization
         ProcessAllPredefines();

			m_shaderCompiledSuccessfully = false;
			const bool bShaderLoadedSuccessfully = LoadShadersSourceToGpu();
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
#if DEBUG
               if (!bParentAccessUniformLocationsInvoked)
                  throw std::exception("Didn't called parent method AccessAllUniformLocations in derived method.");
#endif
            }
			}
		}

      ShaderParams Shader::GetShaderParams() const
      {
         return m_shaderParams;
      }

      void Shader::AccessAllUniformLocations(uint32_t shaderProgramId)
      {
#if DEBUG
         Shader::bParentAccessUniformLocationsInvoked = true;

         if (m_shaderCompiledSuccessfully)
         {
            std::cout << GetCompileLogInfo();
            std::cout << GetLinkLogInfo();
         }
#endif
      }

      bool Shader::LoadShadersSourceToGpu()
      {
         std::string vsSourcePath = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.VertexShaderFile);
         std::string fsSourcePath = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.FragmentShaderFile);
         std::string gsSourcePath = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.GeometryShaderFile);

         auto vsSource = LoadShaderSource(vsSourcePath);
         auto fsSource = LoadShaderSource(fsSourcePath);
         auto gsSource = LoadShaderSource(gsSourcePath);

         ProcessShaderIncludes(vsSource);
         ProcessShaderIncludes(gsSource);
         ProcessShaderIncludes(fsSource);

         return SendToGpuShadersSources(vsSource, fsSource, gsSource);
      }

		void Shader::ProcessAllPredefines()
		{
         std::vector<ShaderGenericDefineConstant> vertexConstantPredefine, fragmentConstantPredefine, geometryConstantPredefine;
         std::vector<ShaderGenericDefine> vertexPredefine, fragmentPredefine, geometryPredefine;
         
         if (m_defineConstantParameters.size() > 0 || m_defines.size() > 0)
         {
            for (auto define_it = m_defineConstantParameters.begin(); define_it != m_defineConstantParameters.end(); ++define_it)
            {
               if (define_it->m_ShaderType & ShaderType::VertexShader)
               {
                  vertexConstantPredefine.emplace_back(*define_it);
               }
               if (define_it->m_ShaderType & ShaderType::FragmentShader)
               {
                  fragmentConstantPredefine.emplace_back(*define_it);
               }
               if (define_it->m_ShaderType & ShaderType::GeometryShader)
               {
                  geometryConstantPredefine.emplace_back(*define_it);
               }
            }

            for (auto define_it = m_defines.begin(); define_it != m_defines.end(); ++define_it)
            {
               if (define_it->m_ShaderType & ShaderType::VertexShader)
               {
                  vertexPredefine.emplace_back(*define_it);
               }
               if (define_it->m_ShaderType & ShaderType::FragmentShader)
               {
                  fragmentPredefine.emplace_back(*define_it);
               }
               if (define_it->m_ShaderType & ShaderType::GeometryShader)
               {
                  geometryPredefine.emplace_back(*define_it);
               }
            }
         }

			if (vertexConstantPredefine.size() > 0 || vertexPredefine.size() > 0)
			{
				if (m_shaderParams.VertexShaderFile != "")
				{
					std::string vsSourcePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.VertexShaderFile));
               ProcessPredefineToFile(vsSourcePath, vertexConstantPredefine, vertexPredefine);
				}
			}
			if (fragmentConstantPredefine.size() > 0 || fragmentPredefine.size() > 0)
			{
				if (m_shaderParams.FragmentShaderFile != "")
				{
					std::string fsSourcePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.FragmentShaderFile));
               ProcessPredefineToFile(fsSourcePath, fragmentConstantPredefine, fragmentPredefine);
				}
			}
			if (geometryConstantPredefine.size() > 0 || geometryPredefine.size() > 0)
			{
				if (m_shaderParams.GeometryShaderFile != "")
				{
					std::string gsSourcePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.GeometryShaderFile));
               ProcessPredefineToFile(gsSourcePath, geometryConstantPredefine, geometryPredefine);
				}
			}
		}

		int32_t Shader::GetSubroutineIndex(ShaderType shaderType, const std::string& subroutineName) const
		{
			return glGetSubroutineIndex(m_shaderProgramID, (GLenum)shaderType, subroutineName.c_str());
		}

		void Shader::LoadSubroutineIndex(ShaderType shaderType, int32_t countIndices, int32_t subroutineIndex) const
		{
			glUniformSubroutinesuiv((int32_t)shaderType, countIndices, (uint32_t*)subroutineIndex);
		}

#if DEBUG

		bool Shader::RecompileShader()
		{
			CleanUp(false);
         LoadShadersSourceToGpu();
			m_shaderCompiledSuccessfully = CompileShaders();
			if (m_shaderCompiledSuccessfully)
			{
				LinkShaders();
				AccessAllUniformLocations(m_shaderProgramID);
			}

         std::cout << "Shader " + m_shaderParams.ShaderName + (m_shaderCompiledSuccessfully ? " has recompiled successfully " : "has not recompiled") << std::endl;

			return m_shaderCompiledSuccessfully;
		}

#endif

      void Shader::Define(ShaderType shaderType, const std::string& name)
      {
         m_defines.emplace_back(ShaderDefine(name, true, shaderType));
      }

      void Shader::Undefine(ShaderType shaderType, const std::string& name)
      {
         m_defines.emplace_back(ShaderDefine(name, false, shaderType));
      }

	}

}