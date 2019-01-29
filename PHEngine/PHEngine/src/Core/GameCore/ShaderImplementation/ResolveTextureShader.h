#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

#include <string>
#include <stdint.h>

using namespace Graphics::OpenGL;

namespace Game
{
	namespace ShaderImpl
	{

		class ResolveTextureShader : public ShaderBase
		{
			using Base = ShaderBase;

			Uniform u_textureSampler;

		public:

			ResolveTextureShader(std::string&& vsPath, std::string&& fsPath);

			virtual ~ResolveTextureShader();

			void SetTextureShader(int32_t slot);

		protected:

			virtual void AccessAllUniformLocations() override;

			virtual void SetShaderPredefine() override;
		};

	}
}

