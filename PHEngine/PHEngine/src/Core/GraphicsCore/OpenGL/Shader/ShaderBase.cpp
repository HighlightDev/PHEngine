#include "ShaderBase.h"

#include <iostream>

namespace Graphics
{
	namespace OpenGL
	{
		ShaderBase::ShaderBase(const ShaderParams& params)
			: Shader(params)
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