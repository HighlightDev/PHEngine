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

			Texture2d(std::string& pathToTex, ITextureMipMapState* mipmapState);

			Texture2d(uint32_t texDescriptor, glm::ivec2 texBufferWH);

         Texture2d(TexParams&& textureParameters);

         Texture2d(TexParams& textureParameters);

			virtual ~Texture2d();

			void BindTexture(uint32_t textureSlot)
			{
				glActiveTexture(GL_TEXTURE0 + textureSlot);
				glBindTexture(m_textureParams.TexTarget, m_texDescriptor);
			}

			void UnbindTexture(uint32_t textureSlot)
			{
				glActiveTexture(GL_TEXTURE0 + textureSlot);
				glBindTexture(m_textureParams.TexTarget, 0);
			}

			void CleanUp();

			inline uint32_t GetTextureDescriptor() {

				return m_texDescriptor;
			}

			inline glm::ivec2 GetTextureRezolution() {

				return glm::ivec2(m_textureParams.TexBufferWidth, m_textureParams.TexBufferHeight);
			}

			inline TexParams GetTextureParameters() {

				return m_textureParams;
			}

		private:

         void InitEmptyTexture();

			uint32_t LoadTextureFromFile(std::string& pathToTex, int32_t texWrapMode = GL_REPEAT);

			uint32_t CreateTexture(const void* pixelsData);
		};

	}
}

