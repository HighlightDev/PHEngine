#pragma once

#include <stdint.h>

namespace Graphics
{
	namespace Renderer
	{

		class DeferredRenderer
		{
			int32_t m_windowWidth;
			int32_t m_windowHeight;

			uint32_t m_depthBuffer;
			uint32_t m_positionBuffer;
			uint32_t m_normalBuffer;
			uint32_t m_albedoWithSpecularBuffer;

			uint32_t m_gBufferFBO;

		public:
			DeferredRenderer(int32_t windowWidth, int32_t windowHeight);

			~DeferredRenderer();

			void PrepareDR();

			void StopDR();

			void BindDepthTexture(int32_t slot);

			void BindPositionTexture(int32_t slot);

			void BindNormalTexture(int32_t slot);

			void BindAlbedoWithSpecularTexture(int32_t slot);

		private:

			void Init();

			void DestroyFramebuffer();

		};

	}
}

