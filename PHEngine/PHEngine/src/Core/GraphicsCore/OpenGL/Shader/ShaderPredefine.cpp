#include "ShaderPredefine.h"

#include <xutility>

namespace Graphics
{
	namespace OpenGL
	{
      /* ************** ShaderDefineConstant ************** */
		ShaderDefineConstant::ShaderDefineConstant(const std::string& name, std::string value, ShaderType shaderType) :
			m_Name(name)
			, m_Value(std::move(value))
			, m_ShaderType(shaderType)
		{
		}

		ShaderDefineConstant::ShaderDefineConstant(const std::string& name, std::string value) :
			m_Name(name)
			, m_Value(value)
			, m_ShaderType(ShaderType::VertexShader)
		{
		}

		ShaderDefineConstant::~ShaderDefineConstant()
		{

		}
      /* ************** ShaderDefine ************** */

      ShaderDefine::ShaderDefine(const std::string& name, const bool isDefined, ShaderType shaderType)
         : m_Name(name)
         , bDefined(isDefined)
         , m_ShaderType(shaderType)
      {
      }
	}
}
