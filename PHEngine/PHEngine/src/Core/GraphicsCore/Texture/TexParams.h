#pragma once

#include <stdint.h>
#include <gl/glew.h>

namespace Graphics
{
	namespace Texture
	{

		struct TexParams
		{
			int32_t TexBufferWidth;
			int32_t TexBufferHeight;
			int32_t TexTarget;
			int32_t TexMagFilter;
			int32_t TexMinFilter;
			int32_t TexMipLvl;
			int32_t TexPixelInternalFormat;
			int32_t TexPixelFormat;
			int32_t TexPixelType;
			int32_t TexWrapMode;

			TexParams(
				int32_t texBufferWidth,
				int32_t texBufferHeight,
				int32_t texTarget = GL_TEXTURE_2D,
				int32_t magFilter = GL_LINEAR,
				int32_t minFilter = GL_LINEAR,
				int32_t texMipLvl = 0,
				int32_t texPixelInternalFormat = GL_RGB,
				int32_t texPixelFormat = GL_RGB,
				int32_t texPixelType = GL_UNSIGNED_BYTE,
				int32_t texWrapMode = GL_REPEAT);

			TexParams();

			~TexParams();
		};

	}
}

