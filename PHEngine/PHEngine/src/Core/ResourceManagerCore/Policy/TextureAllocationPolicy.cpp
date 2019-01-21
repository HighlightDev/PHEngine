#include "TextureAllocationPolicy.h"
#include "Core/UtilityCore/StringExtendedFunctions.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/GraphicsCore/Texture/CubemapTexture.h"

namespace Resources
{

	TextureAllocationPolicy::TextureAllocationPolicy()
	{
	}


	TextureAllocationPolicy::~TextureAllocationPolicy()
	{
	}

	std::shared_ptr<ITexture> TextureAllocationPolicy::AllocateMemory(std::string& arg)
	{
		std::shared_ptr<ITexture> resultTexture;
		std::vector<std::string> pathToTextures = EngineUtility::Split(arg, ',');
		
		switch (pathToTextures.size())
		{
		case 1:
		{
			std::string absolutePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(arg));
			resultTexture = std::shared_ptr<ITexture>(LoadTexture2dFromFile(absolutePath));
			break;
		}
		case 6:
		{
			resultTexture = std::shared_ptr<ITexture>(LoadTextureCubeFromFile(pathToTextures));
			break;
		}
		default: throw  std::invalid_argument("Undefined count of files.");
		}
		return resultTexture;
	}

	void TextureAllocationPolicy::DeallocateMemory(std::shared_ptr<ITexture> arg)
	{
		arg->CleanUp();
	}

	ITexture* TextureAllocationPolicy::LoadTexture2dFromFile(std::string& pathToFile)
	{
		return new Texture2d(pathToFile, new TextureAnisotropy(8.0f));
	}

	ITexture* TextureAllocationPolicy::LoadTextureCubeFromFile(std::vector<std::string>& pathToFiles)
	{
		std::vector<std::string> absolutePaths;

		for (auto it = pathToFiles.begin(); it != pathToFiles.end(); ++it)
		{
			std::string absolutePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(*it));
			absolutePaths.emplace_back(std::move(absolutePath));
		}

		return new CubemapTexture(absolutePaths);
	}

}
