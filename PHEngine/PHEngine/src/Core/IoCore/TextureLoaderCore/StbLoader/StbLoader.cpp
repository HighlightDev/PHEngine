#include "StbLoader.h"

namespace Io
{
	namespace Texture
	{
		namespace Stb
		{

			StbLoader* StbLoader::m_instance = nullptr;

			StbLoader::StbLoader()
			{
			}

			StbLoader::~StbLoader()
			{
				delete m_instance;
				m_instance = nullptr;
			}

			uint8_t* StbLoader::LoadTextureFromFile(std::string& pathToFile)
			{
				//char const *filename, int *x, int *y, int *channels_in_file, int desired_channels
				int32_t width, height, components;
				uint8_t* texData = stbi_load(pathToFile.c_str(), &width, &height, &components, STBI_rgb_alpha);

				return texData;
			}

		}
	}
}
