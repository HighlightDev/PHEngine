#include "Texture2d.h"

#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/IoCore/TextureLoaderCore/StbLoader/StbLoader.h"

namespace Graphics
{
	namespace Texture
	{

		Texture2d::Texture2d(std::string& pathToTex, ITextureMipMapState& mipmapState) 
			: ITexture()
			, m_mipmapState(&mipmapState)
		{
			m_texDescriptor = LoadTextureFromFile(pathToTex);
		}

		Texture2d::Texture2d(int32_t texDescriptor, glm::ivec2 texBufferWH) 
			: ITexture()
			, m_mipmapState(nullptr)
		{

		}

		Texture2d::Texture2d(TexParams&& textureParameters)
			: ITexture()
			, m_mipmapState(nullptr)
		{

		}

		Texture2d::~Texture2d()
		{
			delete &m_mipmapState;
		}

		int32_t Texture2d::LoadTextureFromFile(std::string& pathToTex, int32_t texWrapMode)
		{
			std::string absolutePathToTex = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(pathToTex));

			uint8_t* data = Io::Images::Stb::StbLoader::GetInstance().LoadTextureFromFile(absolutePathToTex, m_textureParams);

			return CreateTexture(data);
		}

		int32_t Texture2d::CreateTexture(const void* pixelsData)
		{
			uint32_t texObject = -1;
			int32_t& textureTarget = m_textureParams.TexTarget;

			glGenTextures(1, &texObject);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glBindTexture(textureTarget, texObject);

			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, m_textureParams.TexWrapMode);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, m_textureParams.TexWrapMode);
			glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, m_textureParams.TexMagFilter);
			if (!m_mipmapState)
			{
				glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, m_textureParams.TexMinFilter);
			}
			else
			{
				m_mipmapState->ExecuteTextureSampleFilteringInstructions();
			}
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			glTexImage2D(textureTarget, 0, m_textureParams.TexPixelInternalFormat, m_textureParams.TexBufferWidth, m_textureParams.TexBufferHeight, 0, m_textureParams.TexPixelFormat, m_textureParams.TexPixelFormat, pixelsData);

			return texObject;
		}

	}
}