#include "Shader.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/UtilityCore/StringExtendedFunctions.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"

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
		template void Shader::Predefine<glm::vec2>(ShaderType, const std::string&, glm::vec2&&);
		template void Shader::Predefine<glm::vec3>(ShaderType, const std::string&, glm::vec3&&);
		template void Shader::Predefine<glm::vec4>(ShaderType, const std::string&, glm::vec4&&);
		template void Shader::Predefine<glm::mat2>(ShaderType, const std::string&, glm::mat2&&);
		template void Shader::Predefine<glm::mat3>(ShaderType, const std::string&, glm::mat3&&);
		template void Shader::Predefine<glm::mat4>(ShaderType, const std::string&, glm::mat4&&);
		template void Shader::Predefine<int32_t>(ShaderType, const std::string&, int32_t&&);
		template void Shader::Predefine<float>(ShaderType, const std::string&, float&&);
		template void Shader::Predefine<double>(ShaderType, const std::string&, double&&);

#if DEBUG
      bool Shader::bParentAccessUniformLocationsInvoked = false;
#endif

      Shader::Shader(const ShaderParams& params)
         : m_shaderParams(params)
         , m_vertexShaderID(-1)
         , m_fragmentShaderID(-1)
         , m_geometryShaderID(-1)
         , m_shaderProgramID(-1)
      {
      }

		Shader::~Shader()
		{
		}

		void Shader::Init()
		{
			SetShaderPredefine(); // start precompile shader customization
			PreprocessorEdit();
			m_shaderCompiledSuccessfully = false;
			const bool bShaderLoadedSuccessfully = LoadShadersSource();
			if (bShaderLoadedSuccessfully)
			{
				m_shaderCompiledSuccessfully = CompileShaders();
			}

			m_shaderProgramID = glCreateProgram();
			if (m_shaderCompiledSuccessfully)
			{
				const bool bLinkedSuccessfully = LinkShaders();
            if (bLinkedSuccessfully)
            {
               AccessAllUniformLocations();
#if DEBUG
               if (!bParentAccessUniformLocationsInvoked)
                  throw std::exception("Didn't called parent method AccessAllUniformLocations in derived method.");
#endif
            }
			}
		}

		bool Shader::LoadSingleShaderSource(int32_t shaderId, std::string& shaderPath)
		{
			bool bLoadResult = false;

			try
			{
				std::ifstream stream(shaderPath);
				std::string line = std::string("");

				while (stream.is_open() && getline(stream, line))
				{
					EngineUtility::StringStreamWrapper::ToString(line, '\n');
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

		bool Shader::LoadShadersSource()
		{
         bool bVertexShaderLoaded = false, bFragmentShaderLoaded = false, bGeometryShaderLoaded = false;

         if (m_shaderParams.VertexShaderFile != "")
         {
            /*Vertex shader load*/
            m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            std::string vsSource = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.VertexShaderFile);
            bVertexShaderLoaded = LoadSingleShaderSource(m_vertexShaderID, vsSource);
         }
         else
         {
            bVertexShaderLoaded = true;
         }

         if (m_shaderParams.FragmentShaderFile != "")
         {
            /*Fragment shader load*/
            m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
            std::string fsSource = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.FragmentShaderFile);
            bFragmentShaderLoaded = LoadSingleShaderSource(m_fragmentShaderID, fsSource);
         }
         else
         {
            bFragmentShaderLoaded = true;
         }

         if (m_shaderParams.GeometryShaderFile != "")
         {
            /*Geometry shader load*/
            m_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
            std::string gsSource = EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.GeometryShaderFile);
            bGeometryShaderLoaded = LoadSingleShaderSource(m_geometryShaderID, gsSource);
         }
         else
         {
            bGeometryShaderLoaded = true;
         }

         return bVertexShaderLoaded && bFragmentShaderLoaded && bGeometryShaderLoaded;
		}

		bool Shader::CompileShaders()
		{
			glCompileShader(m_vertexShaderID);
			glCompileShader(m_fragmentShaderID);
			glCompileShader(m_geometryShaderID);

			return true;
		}

		bool Shader::LinkShaders()
		{
			glAttachShader(m_shaderProgramID, m_vertexShaderID);
			glAttachShader(m_shaderProgramID, m_fragmentShaderID);
			if (m_geometryShaderID != -1)
			{
				glAttachShader(m_shaderProgramID, m_geometryShaderID);
			}
			glLinkProgram(m_shaderProgramID);

			glDetachShader(m_shaderProgramID, m_vertexShaderID);
			glDetachShader(m_shaderProgramID, m_fragmentShaderID);
			if (m_geometryShaderID != -1)
			{
				glDetachShader(m_shaderProgramID, m_geometryShaderID);
			}

			return true;
		}

		void Shader::ProcessPredefine(ShaderType shaderType, std::string& pathToShader)
		{
			if (pathToShader == "")
				return;

			std::ifstream stream(pathToShader);
			std::string line;

			std::vector<std::string> code;

			while (stream.is_open() && getline(stream, line))
			{
				code.emplace_back(line);
			}

			stream.clear();
			stream.close();

			// code only with macros
			std::vector<ShaderPredefine> macros;

			for (auto it = code.begin(); it != code.end(); ++it)
			{
				if (EngineUtility::StartsWith(*it, "#define"))
				{
					size_t indexName = EngineUtility::IndexOf(*it, " ");
					size_t indexValue = EngineUtility::IndexOf(*it, " ", indexName + 1);

					std::string name = it->substr(indexName + 1, indexValue - indexName - 1);
					std::string value = it->substr(indexValue + 1);
					macros.emplace_back(std::move<ShaderPredefine>(std::move(ShaderPredefine(std::move(name), std::move(value)))));
				}
			}

			// remove all macros from code
			for (auto it = code.begin(); it != code.end();)
			{
				if (EngineUtility::StartsWith(*it, "#define"))
					it = code.erase(it);
				else
					++it;
			}

			// get macros only for current shader
			std::vector<ShaderPredefine> input;
			for (auto it = m_defineParameters.begin(); it != m_defineParameters.end(); ++it)
			{
				if ((it->m_ShaderType & ShaderType::VertexShader) == shaderType ||
					(it->m_ShaderType & (int32_t)ShaderType::FragmentShader) == shaderType ||
					(it->m_ShaderType & (int32_t)ShaderType::GeometryShader) == shaderType)
					input.emplace_back(std::move(*it));
			}

			// update values for existing macros
			for (auto input_it = input.begin(); input_it != input.end(); ++input_it)
			{
				for (auto macro_it = macros.begin(); macro_it != macros.end();)
				{
					if (macro_it->m_Name == input_it->m_Name)
					{
						macro_it = macros.erase(macro_it);
					}
					else
					{
						++macro_it;
					}
				}

				macros.emplace_back(std::move(*input_it));
			}


			for (auto it = macros.begin(); it != macros.end(); ++it)
			{
				EngineUtility::StringStreamWrapper::ToString("#define ", it->m_Name, " ", it->m_Value, '\n');
			}

			std::string macroResult = std::move(EngineUtility::StringStreamWrapper::FlushString());

			std::vector<std::string>::iterator version_it = code.begin();

			for (auto it = code.begin(); it != code.end(); ++it, ++version_it)
			{
				if (EngineUtility::StartsWith(*it, "#version"))
				{
					version_it += 2;
					break;
				}
			}

			code.insert(version_it, macroResult);

			std::string codeResult = std::move("");

			for (std::vector<std::string>::iterator it = code.begin(); it != code.end(); ++it)
			{
				std::string& str = *it;
				str = EngineUtility::TrimEnd(str);

				if (code.end() - 1 != it)
					codeResult += str + "\n";
				else
					codeResult += str;
			}

			std::ofstream writeStream(pathToShader);
			writeStream << codeResult;
		}

		void Shader::ProcessAllPredefines()
		{
			bool bEditVertexShader = false, bEditFragmentShader = false, bEditGeometryShader = false;

			if (m_defineParameters.size() > 0)
			{
				for (auto define_it = m_defineParameters.begin(); define_it != m_defineParameters.end(); ++define_it)
				{
					if ((define_it->m_ShaderType & ShaderType::VertexShader))
						bEditVertexShader = true;
					if (define_it->m_ShaderType & ShaderType::FragmentShader)
						bEditFragmentShader = true;
					if (define_it->m_ShaderType & ShaderType::GeometryShader)
						bEditGeometryShader = true;
				}
			}

			if (bEditVertexShader)
			{
				if (m_shaderParams.VertexShaderFile != "")
				{
					std::string vsSource = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.VertexShaderFile));
					ProcessPredefine(ShaderType::VertexShader, vsSource);
				}
			}
			if (bEditFragmentShader)
			{
				if (m_shaderParams.FragmentShaderFile != "")
				{
					std::string fsSource = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.FragmentShaderFile));
					ProcessPredefine(ShaderType::FragmentShader, fsSource);
				}
			}
			if (bEditGeometryShader)
			{
				if (m_shaderParams.GeometryShaderFile != "")
				{
					std::string gsSource = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(m_shaderParams.GeometryShaderFile));
					ProcessPredefine(ShaderType::GeometryShader, gsSource);
				}
			}
		}

		int32_t Shader::GetSubroutineIndex(ShaderType shaderType, std::string&& subroutineName) const
		{
			return glGetSubroutineIndex(m_shaderProgramID, (GLenum)shaderType, subroutineName.c_str());
		}

		Uniform Shader::GetUniform(std::string&& uniformName) const
		{
			try
			{
				return Uniform(m_shaderProgramID, std::move(uniformName));
			}
			catch (std::invalid_argument innerEx)
			{
				std::string message;
				EngineUtility::StringStreamWrapper::ToString("Shader with name", m_shaderParams.ShaderName, " could not bind uniform(s);", "Inner exception message :", '\n', innerEx.what());

				throw std::invalid_argument(EngineUtility::StringStreamWrapper::FlushString());
			}
		}

      UniformArray Shader::GetUniformArray(std::string&& uniformName, size_t countOfUniforms) const
      {
         try
         {
            return UniformArray(m_shaderProgramID, countOfUniforms, std::move(uniformName));
         }
         catch (std::invalid_argument innerEx)
         {
            std::string message;
            EngineUtility::StringStreamWrapper::ToString("Shader with name", m_shaderParams.ShaderName, " could not bind uniform(s);", "Inner exception message :", '\n', innerEx.what());

            throw std::invalid_argument(EngineUtility::StringStreamWrapper::FlushString());
         }
      }

		void Shader::LoadSubroutineIndex(ShaderType shaderType, int32_t countIndices, int32_t subroutineIndex)
		{
			glUniformSubroutinesuiv((int32_t)shaderType, countIndices, (uint32_t*)subroutineIndex);
		}

		std::string Shader::GetCompileLogInfo() const
		{
			std::string compileLog;

			EngineUtility::StringStreamWrapper::FlushString(); // Just to clear stream
			GLint vertex_compiled = 0;
         if (m_vertexShaderID != -1)
         {
            glGetShaderiv(m_vertexShaderID, GL_COMPILE_STATUS, &vertex_compiled);
            if (vertex_compiled != GL_TRUE)
            {
               GLint log_length = 0;
               glGetShaderiv(m_vertexShaderID, GL_INFO_LOG_LENGTH, &log_length);

               std::vector<char> message(log_length);
               glGetShaderInfoLog(m_vertexShaderID, log_length, nullptr, message.data());
               EngineUtility::StringStreamWrapper::ToString("Vertex shader : ", std::string(message.begin(), message.end()), '\n');
            }
         }

			/*Fragment shader log info*/
			GLint fragment_compiled = 0;
         if (m_fragmentShaderID != -1)
         {
            glGetShaderiv(m_fragmentShaderID, GL_COMPILE_STATUS, &fragment_compiled);
            if (fragment_compiled != GL_TRUE)
            {
               GLint log_length = 0;
               glGetShaderiv(m_fragmentShaderID, GL_INFO_LOG_LENGTH, &log_length);

               std::vector<char> message(log_length);
               glGetShaderInfoLog(m_fragmentShaderID, log_length, nullptr, message.data());
               EngineUtility::StringStreamWrapper::ToString("Fragment shader : ", std::string(message.begin(), message.end()), '\n');
            }
         }


			GLint geometry_compiled = 0;
			/*Geometry shader log info*/
			if (m_geometryShaderID != -1)
			{
				glGetShaderiv(m_geometryShaderID, GL_COMPILE_STATUS, &geometry_compiled);
				if (geometry_compiled != GL_TRUE)
				{
					GLint log_length = 0;
					glGetShaderiv(m_geometryShaderID, GL_INFO_LOG_LENGTH, &log_length);

					std::vector<char> message(log_length);
					glGetShaderInfoLog(m_geometryShaderID, log_length, nullptr, message.data());
					EngineUtility::StringStreamWrapper::ToString("Geometry shader : ", std::string(message.begin(), message.end()), '\n');
				}
			}

			if ((m_vertexShaderID != -1 && vertex_compiled != GL_TRUE) || (m_fragmentShaderID != -1 && fragment_compiled != GL_TRUE) || (m_geometryShaderID != -1 && geometry_compiled != GL_TRUE))
				compileLog += std::string("Unresolved mistakes at :" + m_shaderParams.ShaderName + '\n') + EngineUtility::StringStreamWrapper::FlushString() + "\n";

			return compileLog;
		}

		std::string Shader::GetLinkLogInfo() const
		{
			std::string  linkLog;
			EngineUtility::StringStreamWrapper::FlushString(); // Just to clear stream
			GLint isLinked = 0;
			glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &isLinked);

			if (isLinked == GL_FALSE)
			{
				GLint log_length = 0;
				glGetProgramiv(m_shaderProgramID, GL_INFO_LOG_LENGTH, &log_length);

				std::vector<char> message(log_length);
				glGetProgramInfoLog(m_shaderProgramID, log_length, nullptr, message.data());
				EngineUtility::StringStreamWrapper::ToString("Unsolved mistakes at :", m_shaderParams.ShaderName, "\n");
			}

			linkLog = std::move(EngineUtility::StringStreamWrapper::FlushString());
         linkLog = linkLog != "" ? linkLog + "\n" : linkLog;

			return linkLog;
		}

		void Shader::ExecuteShader()
		{
			glUseProgram(m_shaderProgramID);
		}

		void Shader::StopShader()
		{
			glUseProgram(0);
		}

		void Shader::CleanUp(bool bDeleteShaderProgram)
		{
			StopShader();
			glDetachShader(m_shaderProgramID, m_vertexShaderID);
			glDetachShader(m_shaderProgramID, m_fragmentShaderID);
			if (m_geometryShaderID != -1)
			{
				glDetachShader(m_shaderProgramID, m_geometryShaderID);
				glDeleteShader(m_geometryShaderID);
			}

			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			if (bDeleteShaderProgram)
			{
				glDeleteShader(m_shaderProgramID);
			}
		}

		void Shader::PreprocessorEdit()
		{
			ProcessAllPredefines();
			m_defineParameters.clear();
		}

#if DEBUG

		bool Shader::RecompileShader()
		{
			CleanUp(false);
			LoadShadersSource();
			m_shaderCompiledSuccessfully = CompileShaders();
			if (m_shaderCompiledSuccessfully)
			{
				LinkShaders();
				AccessAllUniformLocations();
			}

         std::cout << "Shader " + m_shaderParams.ShaderName + (m_shaderCompiledSuccessfully ? " has recompiled successfully " : "has not recompiled") << std::endl;

			return m_shaderCompiledSuccessfully;
		}

#endif

		template <typename ValueType>
		void Shader::Predefine(ShaderType shaderType, const std::string& name, ValueType&& value)
		{
			std::string formatedValue = MacroConverter<ValueType>::GetValue(std::forward<ValueType>(value));
			m_defineParameters.emplace_back(ShaderPredefine(name, formatedValue, shaderType));
		}

	}

}