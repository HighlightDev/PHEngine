#include "TexParams.h"

namespace Graphics
{
	namespace Texture
	{

		TexParams::TexParams(int32_t texBufferWidth,
			int32_t texBufferHeight,
			int32_t texTarget,
			int32_t magFilter,
			int32_t minFilter,
			int32_t texMipLvl,
			int32_t texPixelInternalFormat,
			int32_t texPixelFormat,
			int32_t texPixelType,
			int32_t texWrapMode) :
			TexBufferWidth(texBufferWidth)
			, TexBufferHeight(texBufferHeight)
			, TexTarget(texTarget)
			, TexMagFilter(magFilter)
			, TexMinFilter(minFilter)
			, TexMipLvl(texMipLvl)
			, TexPixelInternalFormat(texPixelInternalFormat)

			, TexPixelFormat(texPixelFormat)
			, TexPixelType(texPixelType)
			, TexWrapMode(texWrapMode)
		{
		}

		TexParams::TexParams()
			: TexParams(0, 0)
		{
		}

		TexParams::~TexParams()
		{
		}

	}
}
