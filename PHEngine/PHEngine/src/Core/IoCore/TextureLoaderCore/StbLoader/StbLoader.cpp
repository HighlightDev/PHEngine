#define STB_IMAGE_IMPLEMENTATION
#include "StbLoader.h"

namespace Io
{
	namespace Images
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

			uint8_t* StbLoader::LoadTextureFromFile(std::string& pathToFile, Graphics::Texture::TexParams& out_params)
			{
				int32_t width, height, components;

				uint8_t* texData = stbi_load(pathToFile.c_str(), &width, &height, &components, STBI_rgb_alpha);

				out_params.TexBufferWidth = width;
				out_params.TexBufferHeight = height;

				if (components == 3)
					out_params.TexPixelType = GL_RGB;
				else if (components == 4)
					out_params.TexPixelFormat = GL_RGBA;

				return texData;
			}

		}
	}
}
