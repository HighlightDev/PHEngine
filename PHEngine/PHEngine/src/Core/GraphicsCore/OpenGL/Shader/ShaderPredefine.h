#pragma once
#include <string>

#include "ShaderType.h"

namespace Graphics
{
	namespace OpenGL
	{
		struct ShaderDefineConstant
		{
			std::string m_Name;
			std::string m_Value;
			ShaderType m_ShaderType;

			ShaderDefineConstant(const std::string& name, std::string value);

			ShaderDefineConstant(const std::string& name, std::string value, ShaderType shaderType);

			~ShaderDefineConstant();
		};

      struct ShaderDefine
      {
         std::string m_Name;
         bool bDefined;
         ShaderType m_ShaderType;

         ShaderDefine(const std::string& name, const bool isDefined, ShaderType shaderType);
      };
	}
}

