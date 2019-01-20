#include "TextureAllocationPolicy.h"
#include "Core/UtilityCore/StringExtendedFunctions.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"

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

		std::string absolutePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(arg));
		std::vector<std::string> pathToTexture = EngineUtility::Split(absolutePath, ',');
		switch (pathToTexture.size())
		{
		case 1:
		{
			resultTexture = std::shared_ptr<ITexture>(LoadTexture2dFromFile(pathToTexture[0]));
			break;
		}
		case 6:
		{
			resultTexture = std::shared_ptr<ITexture>(LoadTextureCubeFromFile(pathToTexture));
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
		return nullptr;
		//return new CubemapTexture(pathToFiles);
	}

}
