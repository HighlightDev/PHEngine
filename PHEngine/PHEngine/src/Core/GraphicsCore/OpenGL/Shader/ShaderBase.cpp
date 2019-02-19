#include "ShaderBase.h"

namespace Graphics
{
	namespace OpenGL
	{
		ShaderBase::ShaderBase(std::string&& shaderName, std::string&& vertexShaderFile, std::string&& fragmentShaderFile, std::string&& geometryShaderFile)
			: Shader(std::move(shaderName), std::move(vertexShaderFile), std::move(fragmentShaderFile), std::move(geometryShaderFile))
		{
		}

		void ShaderBase::SetShaderPredefine()
		{
			// Implement in derived shaders
			throw std::exception("Not implemented. Must be overridden by derived classes");
		}

		ShaderBase::~ShaderBase()
		{
		}

		void ShaderBase::AccessAllUniformLocations()
		{
#if DEBUG
         Shader::bParentAccessUniformLocationsInvoked = true;
#endif
			if (m_shaderCompiledSuccessfully)
			{
				std::cout << GetCompileLogInfo() << std::endl;
				std::cout << GetLinkLogInfo() << std::endl;
				//Log.AddToFileStreamLog(getCompileLogInfo(m_shaderName));
				//Log.AddToFileStreamLog(getLinkLogInfo(m_shaderName));
			}
			//else Log.AddToFileStreamLog(DateTime.Now.ToString() + "  " + m_shaderName + " : shader file(s) not found!");
		}
	}
}