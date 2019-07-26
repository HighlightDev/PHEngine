#pragma once
#include "Uniform.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/UtilityCore/StringExtendedFunctions.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "ShaderPredefine.h"

#include <stdint.h>
#include <string>

namespace Graphics
{
   namespace OpenGL
   {
      class IShader
      {
      protected:

         int32_t m_vertexShaderID;
         int32_t m_fragmentShaderID;
         int32_t m_geometryShaderID;
         uint32_t m_shaderProgramID;

         std::string mShaderName;
         bool m_shaderCompiledSuccessfully;

      public:

         IShader(const std::string& shaderName);

         virtual ~IShader();

         Uniform GetUniform(const std::string& uniformName, uint32_t shaderProgramID) const;
         UniformArray GetUniformArray(const std::string& uniformName, size_t countOfUniforms, uint32_t shaderProgramID) const;

         std::string GetCompileLogInfo() const;

         std::string GetLinkLogInfo() const;

         void ExecuteShader() const;

         void StopShader() const;

         void CleanUp(bool bDeleteShaderProgram = true);

         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) = 0;
         virtual void SetShaderPredefine() {};
         virtual void ProcessAllPredefines() {};

         void ProcessPredefineToFile(const std::string& pathToShader, const std::vector<ShaderGenericDefineConstant>& constantDefines, const std::vector<ShaderGenericDefine>& defines) const;
         void ProcessPredefineToSource(std::string& shaderSource, const std::vector<ShaderGenericDefineConstant>& constantDefines, const std::vector<ShaderGenericDefine>& defines) const;
         void WriteShaderSrc(const std::string& pathToShader, const std::string& src) const;

         bool CompileShaders();
         bool LinkShaders() const;

         bool ProcessShaderIncludes(std::string& shaderSource);
         bool SendToGpuSingleShaderSource(int32_t shaderId, const std::string& shaderSource) const;
         bool SendToGpuShadersSources(std::string& vsSource, std::string& fsSource, std::string& gsSource);

         std::vector<std::string> LoadShaderSrcVector(const std::string& pathToShader) const;
         std::string LoadShaderSource(const std::string& pathToShader) const;

      private:

         std::string GetPredefinedSource(std::vector<std::string>& shaderSourceVector, const std::vector<ShaderGenericDefineConstant>& constantDefines, const std::vector<ShaderGenericDefine>& defines) const;
      };
   }
}