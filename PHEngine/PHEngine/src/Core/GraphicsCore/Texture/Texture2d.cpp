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

			uint8_t* data = Io::Images::Stb::StbLoader::GetInstance().LoadTextureFromFile(absolutePathToTex);

			return CreateTexture(0, nullptr, 100, 100);
		}

		int32_t Texture2d::CreateTexture(int32_t format, const void* pixelsData, int32_t width, int32_t height) 
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
				glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, m_textureParams.TexMinFilter);
			else
			{
				//m_mipmapState;
				//if (CheckForAnisotropicTextureFiltering())
				//{
				//	GL.TexParameter(TextureTarget.Texture2D, (TextureParameterName)ExtTextureFilterAnisotropic.TextureMaxAnisotropyExt,
				//		(anisotropyLevel == -1.0f) ? MaxAnisotropy : (anisotropyLevel >= MaxAnisotropy) ? MaxAnisotropy : (anisotropyLevel < 0.0f) ? 0.0f : anisotropyLevel);
				//}
				//GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (Int32)filterType);
				//GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.GenerateMipmap, 1); // 1 stands for TRUE statement
				//GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureLodBias, -0.4f); // might need to use variable to change this value
			}
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			switch (format)
			{
			case 24:
				glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, m_textureParams.TexPixelFormat, GL_RGB, pixelsData);
				break;

			case 32:
				glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, m_textureParams.TexPixelFormat, GL_RGBA, pixelsData);
				break;
			}

			return texObject;
		}

	}
}