#include "CubemapTexture.h"
#include "Core/IoCore/TextureLoaderCore/StbLoader/StbLoader.h"

using namespace Io::Images::Stb;

namespace Graphics
{
	namespace Texture
	{

		CubemapTexture::CubemapTexture(std::vector<std::string>& pathToTextures)
		{
			m_texDescriptor = CreateCubemapTexture(pathToTextures);
		}

		CubemapTexture::~CubemapTexture()
		{
		}

		uint32_t CubemapTexture::CreateCubemapTexture(std::vector<std::string>& pathToTextures)
		{
			uint32_t resultTextureDescriptor = -1;
			size_t mutualPixelFormat = -1;

			glGenTextures(1, &m_texDescriptor);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_texDescriptor);

			const size_t texturesCount = pathToTextures.size();
			for (size_t texIndex = 0; texIndex < texturesCount; texIndex++)
			{
				TexParams texParam;
				uint8_t* texData = StbLoader::GetInstance().AllocateTextureMemoryFromFile(pathToTextures[texIndex], texParam);
				if (mutualPixelFormat == -1)
				{
					mutualPixelFormat = texParam.TexPixelFormat;
				}
				else if (mutualPixelFormat != texParam.TexPixelFormat)
				{
					throw std::invalid_argument("Every texture must have same pixel format.");
				}

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + texIndex, 0, texParam.TexPixelInternalFormat, texParam.TexBufferWidth, texParam.TexBufferHeight, 0, texParam.TexPixelFormat, texParam.TexPixelType, texData);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, texParam.TexMagFilter);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, texParam.TexMinFilter);

				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

				m_texParams.emplace_back(std::move(texParam));

				StbLoader::GetInstance().ReleaseTextureMemory();
			}

			return resultTextureDescriptor;
		}

		void CubemapTexture::BindTexture(uint32_t textureSlot)
		{
			glActiveTexture(GL_TEXTURE0 + textureSlot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_texDescriptor);
		}

		void CubemapTexture::UnbindTexture(uint32_t textureSlot)
		{
			glActiveTexture(GL_TEXTURE0 + textureSlot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		void CubemapTexture::CleanUp()
		{
			glDeleteTextures(1, &m_texDescriptor);
		}

		uint32_t CubemapTexture::GetTextureDescriptor()
		{
			return m_texDescriptor;
		}

		glm::ivec2 CubemapTexture::GetTextureRezolution()
		{
			return glm::ivec2(m_texParams[0].TexBufferWidth, m_texParams[0].TexBufferHeight);
		}

		TexParams CubemapTexture::GetTextureParameters()
		{
			return m_texParams[0];
		}
	}
}
