#pragma once

#include <string>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <xutility>

#include "Core/UtilityCore/StringStreamWrapper.h"

namespace Graphics
{
	namespace OpenGL
	{
		template <typename Type>
		struct MacroConverter
		{
			static std::string GetValue(Type&& value)
			{
				std::string resultStr;
				return std::move(resultStr);
			};
		};

		template <>
		struct MacroConverter<int32_t>
		{
			static std::string&& GetValue(int32_t&& value)
			{
				EngineUtility::StringStreamWrapper::ToString(value);
				return std::move(EngineUtility::StringStreamWrapper::FlushString());
			}
		};

		template <>
		struct MacroConverter<glm::vec2>
		{
			static std::string GetValue(glm::vec2&& value)
			{
				EngineUtility::StringStreamWrapper::ToString("vec2(", value.x, ", ", value.y, ")");
				std::string result = std::move(EngineUtility::StringStreamWrapper::FlushString());
				return result;
			}
		};

		template <>
		struct MacroConverter<glm::vec3>
		{
			static std::string GetValue(glm::vec3&& value)
			{
				EngineUtility::StringStreamWrapper::ToString("vec3(", value.x, ", ", value.y, value.z, ")");
				return std::move(EngineUtility::StringStreamWrapper::FlushString());
			}
		};

		template <>
		struct MacroConverter<glm::vec4>
		{
			static std::string GetValue(glm::vec4&& value)
			{
				EngineUtility::StringStreamWrapper::ToString("vec4(", value.x, ", ", value.y, value.z, value.w, ")");
				return std::move(EngineUtility::StringStreamWrapper::FlushString());
			}
		};
	}
}