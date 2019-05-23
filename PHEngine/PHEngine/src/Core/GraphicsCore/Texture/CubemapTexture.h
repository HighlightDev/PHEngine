#pragma once

#include <vector>
#include <string>

#include "ITexture.h"


namespace Graphics
{
	namespace Texture
	{

		class CubemapTexture :
			public ITexture
		{
			std::vector<TexParams> m_texParams;

		public:

			CubemapTexture(std::vector<std::string>& pathToTextures);

         CubemapTexture(TexParams cubemapTexParams);

			virtual ~CubemapTexture();

			virtual void BindTexture(uint32_t textureSlot) override;

			virtual void UnbindTexture(uint32_t textureSlot) override;

			virtual void CleanUp() override;

			virtual uint32_t GetTextureDescriptor() override;

			virtual glm::ivec2 GetTextureRezolution() override;

			virtual TexParams GetTextureParameters() override;

		private:

			uint32_t CreateCubemapTexture(std::vector<std::string>& pathToTextures);

         uint32_t CreateEmptyCubemapTexture();
		};

	}
}

