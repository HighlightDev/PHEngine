#include "TexturePool.h"

namespace Resources
{

	std::unique_ptr<TexturePool> TexturePool::m_instance;

	TexturePool::TexturePool()
		: PoolBase<ITexture, std::string, TextureAllocationPolicy>()
	{
	}

	TexturePool::~TexturePool()
	{
	}
}