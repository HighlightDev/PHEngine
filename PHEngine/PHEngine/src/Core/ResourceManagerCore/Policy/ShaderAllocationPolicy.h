#pragma once

#include <memory>
#include <string>

#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/UtilityCore/StringExtendedFunctions.h"

using namespace Graphics::OpenGL;
using namespace EngineUtility;

namespace Resources
{

	class ShaderAllocationPolicy
	{
	public:

		ShaderAllocationPolicy() {

		}

		~ShaderAllocationPolicy() {

		}

		template <typename ShaderT>
		static std::shared_ptr<ShaderBase> AllocateMemory(std::string& arg)
		{
			std::vector<std::string> compositeKey = Split(arg, ',');
			ShaderBase* allocatedShader = nullptr;

			/*if (compositeKey.size() > 2)
				allocatedShader = new ShaderT(std::forward<std::string>(compositeKey[0]), std::forward<std::string>(compositeKey[1]), std::forward<std::string>(compositeKey[2]));
			else*/
			allocatedShader = new ShaderT(std::forward<std::string>(compositeKey[0]), std::forward<std::string>(compositeKey[1]));

			return std::shared_ptr<ShaderBase>(allocatedShader);
		}

		static void DeallocateMemory(std::shared_ptr<ShaderBase> arg)
		{
			arg->CleanUp();
		}
	};

}