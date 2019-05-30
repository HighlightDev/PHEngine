#pragma once
#include "Shader.h"

#include <iostream>
#include <utility>

namespace Graphics
{
	namespace OpenGL
	{

		class ShaderBase :
			public Shader
		{
		public:
			
			ShaderBase(const ShaderParams& params);

			~ShaderBase();

		protected:

			virtual void AccessAllUniformLocations() override;

			virtual void SetShaderPredefine() override;
		};

	}
}
