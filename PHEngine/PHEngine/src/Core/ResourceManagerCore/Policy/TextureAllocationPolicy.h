#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Core/GraphicsCore/Texture/ITexture.h"
#include "Core/GraphicsCore/Texture/Texture2d.h"
#include "Core/GraphicsCore/Texture/TexParams.h"

using namespace Graphics::Texture;

namespace Resources
{

	class TextureAllocationPolicy
	{
	public:

		TextureAllocationPolicy();

		~TextureAllocationPolicy();

		static std::shared_ptr<ITexture> AllocateMemory(std::string& arg);

		static void DeallocateMemory(std::shared_ptr<ITexture> arg);

	private:

		static ITexture* LoadTexture2dFromFile(std::string& pathToFile);

		static ITexture* LoadTextureCubeFromFile(std::vector<std::string>& pathToFiles);
	};

}

