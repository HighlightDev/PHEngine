#include "ShaderPredefine.h"

#include <xutility>

namespace Graphics
{
	namespace OpenGL
	{
		ShaderPredefine::ShaderPredefine(std::string&& name, std::string&& value, ShaderType shaderType) :
			m_Name(std::move(name))
			, m_Value(std::move(value))
			, m_ShaderType(shaderType)
		{
		}

		ShaderPredefine::ShaderPredefine(std::string&& name, std::string&& value) :
			m_Name(std::move(name))
			, m_Value(std::move(value))
			, m_ShaderType(ShaderType::VertexShader)
		{
		}

		ShaderPredefine::~ShaderPredefine()
		{

		}
	}
}
