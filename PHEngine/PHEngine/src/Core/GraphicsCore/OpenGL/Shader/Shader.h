#pragma once
#include "ShaderType.h"
#include "ShaderPredefine.h"
#include "Uniform.h"
#include "ShaderPredefineUtility.h"
#include "ShaderParams.h"

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

			Shader(const ShaderParams& params);

			virtual ~Shader();

#if DEBUG
         static bool bParentAccessUniformLocationsInvoked;
#endif

		private:

         ShaderParams m_shaderParams;

			int32_t m_vertexShaderID;
			int32_t m_fragmentShaderID;
			int32_t m_geometryShaderID;
			uint32_t m_shaderProgramID;
			
			std::vector<ShaderDefineConstant> m_defineConstantParameters;
         std::vector<ShaderDefine> m_defines;

		protected:

			bool m_shaderCompiledSuccessfully;

		private:

			bool LoadSingleShaderSourceFromFile(int32_t shaderId, std::string& shaderPath);

			bool LoadShadersSourceFromFile();

			bool CompileShaders();

			bool LinkShaders();

			void ProcessPredefine(ShaderType shaderType, std::string&  pathToShader);

			void ProcessAllPredefines();

		protected:

			virtual void AccessAllUniformLocations() = 0;

			virtual void SetShaderPredefine() = 0;

			// Init should be called in child constructor!
         virtual void ShaderInit();

			Uniform GetUniform(std::string&& uniformName) const;

         UniformArray GetUniformArray(std::string&& uniformName, size_t countOfUniforms) const;

			void LoadSubroutineIndex(ShaderType shaderType, int32_t countIndices, int32_t subroutineIndex);

         int32_t GetSubroutineIndex(ShaderType shaderType, std::string&& subroutineName) const;

			template <typename ValueType>
			void DefineConstant(ShaderType shaderType, const std::string& name, ValueType&& value);

         void Define(ShaderType shaderType, const std::string& name);

         void Undefine(ShaderType shaderType, const std::string& name);

		public:

			std::string GetCompileLogInfo() const;

			std::string GetLinkLogInfo() const;

			void ExecuteShader();

			void StopShader();

			void CleanUp(bool bDeleteShaderProgram = true);

			void PreprocessorEdit();

#if DEBUG
			bool RecompileShader();
#endif

		};
	}
}