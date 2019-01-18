#pragma once

namespace Graphics
{
	namespace Texture
	{
		/************************************************************************/
		/*								ITextureMipMapState                     */
		/************************************************************************/
		class ITextureMipMapState
		{

		public:
			ITextureMipMapState();
			virtual ~ITextureMipMapState();
		};

		/************************************************************************/
		/*								TextureMipMap                           */
		/************************************************************************/
		class TextureMipMap : public ITextureMipMapState
		{

		public:

			TextureMipMap();

			~TextureMipMap();
		};

		/************************************************************************/
		/*								TextureAnisotropy                       */
		/************************************************************************/
		class TextureAnisotropy : public ITextureMipMapState
		{

		public :
			TextureAnisotropy();

			~TextureAnisotropy();
		};

	}
}

