#pragma once

#include "ITexture.h"
#include "TextureMipMapState.h"

#include <string>
#include <gl/glew.h>

namespace Graphics
{
	namespace Texture
	{

		class Texture2d :
			public ITexture
		{
		private:

			TexParams m_textureParams;
			ITextureMipMapState* m_mipmapState;

		public:

			Texture2d(std::string& pathToTex, ITextureMipMapState& mipmapState);

			Texture2d(int32_t texDescriptor, glm::ivec2 texBufferWH);

			Texture2d(TexParams&& textureParameters);

			virtual ~Texture2d();

			void BindTexture(uint32_t textureSlot)
			{
				glBindTexture(m_textureParams.TexTarget, m_texDescriptor);
			}

			void UnbindTexture(uint32_t textureSlot)
			{
				glBindTexture(m_textureParams.TexTarget, 0);
			}

			void CleanUp()
			{
			}

			uint32_t GetTextureDescriptor()
			{
				return m_texDescriptor;
			}

			glm::ivec2 GetTextureRezolution()
			{
				return glm::ivec2();
			}

			TexParams GetTextureParameters()
			{
				return TexParams();
			}

		private:

			int32_t LoadTextureFromFile(std::string& pathToTex, int32_t texWrapMode = GL_REPEAT);

			int32_t CreateTexture(const void* pixelsData);
		};

	}
}

