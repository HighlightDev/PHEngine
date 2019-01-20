#include "Texture2d.h"

#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/IoCore/TextureLoaderCore/StbLoader/StbLoader.h"

using namespace Io::Images::Stb;

namespace Graphics
{
	namespace Texture
	{

		Texture2d::Texture2d(std::string& pathToTex, ITextureMipMapState* mipmapState) 
			: ITexture()
			, m_mipmapState(mipmapState)
		{
			m_texDescriptor = LoadTextureFromFile(pathToTex);
		}

		Texture2d::Texture2d(uint32_t texDescriptor, glm::ivec2 texBufferWH)
			: ITexture()
			, m_mipmapState(nullptr)
		{
			m_texDescriptor = texDescriptor;
			m_textureParams.TexBufferWidth = texBufferWH.x;
			m_textureParams.TexBufferHeight = texBufferWH.y;
		}

		Texture2d::Texture2d(TexParams&& textureParameters)
			: ITexture()
			, m_mipmapState(nullptr)
		{
			m_textureParams = std::move(textureParameters);

			glGenTextures(1, &m_texDescriptor);
			glBindTexture(GL_TEXTURE_2D, m_texDescriptor);

			glTexImage2D(GL_TEXTURE_2D, 0, m_textureParams.TexPixelInternalFormat, m_textureParams.TexBufferWidth, m_textureParams.TexBufferHeight, 0, m_textureParams.TexPixelFormat, m_textureParams.TexPixelType, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_textureParams.TexWrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_textureParams.TexWrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureParams.TexMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureParams.TexMinFilter);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture2d::~Texture2d()
		{
			if (m_mipmapState)
				delete m_mipmapState;
		}

		uint32_t Texture2d::LoadTextureFromFile(std::string& pathToTex, int32_t texWrapMode)
		{
			uint8_t* data = StbLoader::GetInstance().AllocateTextureMemoryFromFile(pathToTex, m_textureParams);
			uint32_t readyToWorkDescriptor = CreateTexture(data);
			StbLoader::GetInstance().ReleaseTextureMemory(); // Release memory allocated for texture
			return readyToWorkDescriptor;
		}

		uint32_t Texture2d::CreateTexture(const void* pixelsData)
		{
			uint32_t texObject = -1;
			int32_t& textureTarget = m_textureParams.TexTarget;

			glGenTextures(1, &texObject);

			glBindTexture(textureTarget, texObject);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, m_textureParams.TexWrapMode);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, m_textureParams.TexWrapMode);
			glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, m_textureParams.TexMagFilter);
			glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, m_textureParams.TexMinFilter);
			if (m_mipmapState)
			{
				m_mipmapState->ExecuteTextureSampleFilteringInstructions();
			}
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			glTexImage2D(textureTarget, 0, m_textureParams.TexPixelInternalFormat,
				m_textureParams.TexBufferWidth, m_textureParams.TexBufferHeight, 0, m_textureParams.TexPixelFormat, m_textureParams.TexPixelType, pixelsData);

			glBindTexture(textureTarget, 0);

			return texObject;
		}

		void Texture2d::CleanUp()
		{
			glDeleteTextures(1, &m_texDescriptor);
		}

	}
}