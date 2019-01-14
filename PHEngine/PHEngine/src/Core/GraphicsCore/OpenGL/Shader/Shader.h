#pragma once
#include "ShaderType.h"
#include "ShaderPredefine.h"
#include "Uniform.h"
#include "ShaderPredefineUtility.h"

#include <string>
#include <stdint.h>
#include <vector>
#include <xutility>

namespace Graphics
{
	namespace OpenGL
	{
		class Shader
		{
		public:

			Shader(std::string&& shaderName, std::string&& vertexShaderFile, std::string&& fragmentShaderFile, std::string&& geometryShaderFile = "");

			virtual ~Shader();

		private:

			std::string m_vsPath;
			std::string m_fsPath;
			std::string m_gsPath;

			int32_t m_vertexShaderID;
			int32_t m_fragmentShaderID;
			int32_t m_geometryShaderID;
			uint32_t m_shaderProgramID;
			bool m_shaderCompiledSuccessfully;
			std::string m_shaderName;

			std::vector<ShaderPredefine> m_defineParameters;

			bool LoadSingleShaderSource(int32_t shaderId, std::string& shaderPath);

			bool LoadShadersSource();

			bool CompileShaders();

			bool LinkShaders();

			void ProcessPredefine(ShaderType shaderType, std::string&  pathToShader);

			void ProcessAllPredefines();

		protected:

			int32_t GetSubroutineIndex(ShaderType shaderType, std::string&& subroutineName);

			virtual void AccessAllUniformLocations() = 0;

			virtual void SetShaderPredefine() = 0;

			// Init should be called in child constructor!
			void Init();

			Uniform GetUniform(std::string&& uniformName);

			void LoadSubroutineIndex(ShaderType shaderType, int32_t countIndices, int32_t subroutineIndex);

			template <typename ValueType>
			void Predefine(ShaderType shaderType, std::string&& name, ValueType&& value);

		public:

			std::string GetCompileLogInfo();

			std::string GetLinkLogInfo();

			void ExecuteShader();

			void StopShader();

			void CleanUp(bool bDeleteShaderProgram = true);

			void PreprocessorEdit();

			bool RecompileShader();

		};
	}
}