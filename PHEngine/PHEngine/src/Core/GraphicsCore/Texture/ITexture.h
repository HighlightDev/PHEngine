#pragma once

#include <stdint.h>
#include <glm/vec2.hpp>

#include "TexParams.h"

namespace Graphics
{
	namespace Texture
	{

		class ITexture
		{
		protected:

			int32_t m_texDescriptor;

		public:
			ITexture();

			virtual ~ITexture();

			virtual void BindTexture(uint32_t textureSlot) = 0;
			virtual void UnbindTexture(uint32_t textureSlot) = 0;
			virtual void CleanUp() = 0;
			virtual uint32_t GetTextureDescriptor() = 0;
			virtual glm::ivec2 GetTextureRezolution() = 0;
			virtual TexParams GetTextureParameters() = 0;
		};

		ITexture::ITexture() 
			: m_texDescriptor(-1)
		{
		}


		ITexture::~ITexture()
		{
		}

	}
}
