#pragma once

#include <string>

namespace EngineUtility
{

	bool StartsWith(std::string& source, const std::string& lookfor);

	size_t IndexOf(std::string& source, const std::string& lookfor, size_t offset = 0);

	size_t LastIndexOf(std::string& source, const std::string& lookfor, size_t offset = 0);

	std::string TrimStart(std::string& source);

	std::string TrimEnd(std::string& source);

}