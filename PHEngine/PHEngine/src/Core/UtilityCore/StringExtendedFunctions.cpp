#include "StringExtendedFunctions.h"

#include <algorithm>
#include <cctype>

namespace EngineUtility
{

	bool StartsWith(std::string& source, const std::string& lookfor)
	{
		return source.find(lookfor) == 0;
	}

	size_t IndexOf(std::string& source, const std::string& lookfor, size_t offset)
	{
		size_t result = std::string::npos;
		typename std::string::size_type location = source.find(lookfor, offset);
		if (location != std::string::npos)
			result = location;

		return result;
	}

	size_t LastIndexOf(std::string& source, const std::string& lookfor, size_t offset)
	{
		size_t index = std::string::npos;
		size_t new_offset = 0;
		do
		{
			new_offset = IndexOf(source, lookfor, new_offset);
			if (new_offset != std::string::npos)
			{
				index = new_offset;
				new_offset += lookfor.size();
			}
		} while (new_offset != std::string::npos);

		return index;
	}

	std::string TrimStart(std::string& sourceStr)
	{
		std::string str = sourceStr;
		auto trim = [](std::string& s) -> void
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), 
				[](int32_t ch) {
				return ! std::isspace(ch); }
			));
		};

		trim(str);
		return str;
	}

	std::string TrimEnd(std::string& sourceStr)
	{
		std::string str = sourceStr;
		auto trim = [](std::string& s) -> void
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch);
			}).base(), s.end());
		};

		trim(str);
		return str;
	}

}