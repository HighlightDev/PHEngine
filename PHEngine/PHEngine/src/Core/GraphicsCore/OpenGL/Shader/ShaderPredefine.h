#pragma once
#include <string>

#include "ShaderType.h"

namespace Graphics
{
	namespace OpenGL
	{
		struct ShaderPredefine
		{
			std::string m_Name;
			std::string m_Value;
			ShaderType m_ShaderType;

			ShaderPredefine(std::string&& name, std::string&& value);

			ShaderPredefine(std::string&& name, std::string&& value, ShaderType shaderType);

			~ShaderPredefine();
		};
	}
}

