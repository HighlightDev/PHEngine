#include "ShaderPredefine.h"

#include <xutility>

namespace Graphics
{
	namespace OpenGL
	{
		ShaderPredefine::ShaderPredefine(const std::string& name, std::string value, ShaderType shaderType) :
			m_Name(name)
			, m_Value(std::move(value))
			, m_ShaderType(shaderType)
		{
		}

		ShaderPredefine::ShaderPredefine(const std::string& name, std::string value) :
			m_Name(name)
			, m_Value(value)
			, m_ShaderType(ShaderType::VertexShader)
		{
		}

		ShaderPredefine::~ShaderPredefine()
		{

		}
	}
}
