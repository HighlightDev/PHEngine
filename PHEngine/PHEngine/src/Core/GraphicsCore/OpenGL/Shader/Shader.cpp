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
			const bool bShaderLoadedSuccessfully = LoadShadersSourceFromFile();
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

		bool Shader::LoadSingleShaderSourceFromFile(int32_t shaderId, std::string& shaderPath)
		{
			bool bLoadResult = false;

			try
			{
            std::vector<std::string> srcVec = LoadShaderSrc(shaderPath);

            for (auto srcLine : srcVec)
            {
               EngineUtility::StringStreamWrapper::ToString(srcLine, '\n');
            }

				std::string shaderSource = std::move(EngineUtility::StringStreamWrapper::FlushString());

				const char *c_str = shaderSource.c_str();
				glShaderSource(shaderId, 1, &c_str, nullptr);

				bLoadResult = true;
			}
			catch (...)
			{

			}

			return bLoadResult;
		}

		bool Shader::LoadShadersSourceFromFile()
		{
         bool bVertexShaderLoaded = true, bFragmentShaderLoaded = true, bGeometryShaderLoaded = true;

         if (m_shaderParams.VertexShaderFile != "")
         {
            /*Vertex shader load*/
            m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            std::string vsSource = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.VertexShaderFile);
            bVertexShaderLoaded &= LoadSingleShaderSourceFromFile(m_vertexShaderID, vsSource);
         }

         if (m_shaderParams.FragmentShaderFile != "")
         {
            /*Fragment shader load*/
            m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
            std::string fsSource = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.FragmentShaderFile);
            bFragmentShaderLoaded &= LoadSingleShaderSourceFromFile(m_fragmentShaderID, fsSource);
         }

         if (m_shaderParams.GeometryShaderFile != "")
         {
            /*Geometry shader load*/
            m_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
            std::string gsSource = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.GeometryShaderFile);
            bGeometryShaderLoaded &= LoadSingleShaderSourceFromFile(m_geometryShaderID, gsSource);
         }

         return bVertexShaderLoaded && bFragmentShaderLoaded && bGeometryShaderLoaded;
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

      void Shader::SetShaderPredefine()
      {

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
					std::string vsSource = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.VertexShaderFile));
					ProcessPredefine(vsSource, vertexConstantPredefine, vertexPredefine);
				}
			}
			if (fragmentConstantPredefine.size() > 0 || fragmentPredefine.size() > 0)
			{
				if (m_shaderParams.FragmentShaderFile != "")
				{
					std::string fsSource = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.FragmentShaderFile));
					ProcessPredefine(fsSource, fragmentConstantPredefine, fragmentPredefine);
				}
			}
			if (geometryConstantPredefine.size() > 0 || geometryPredefine.size() > 0)
			{
				if (m_shaderParams.GeometryShaderFile != "")
				{
					std::string gsSource = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.GeometryShaderFile));
					ProcessPredefine(gsSource, geometryConstantPredefine, geometryPredefine);
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
			LoadShadersSourceFromFile();
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