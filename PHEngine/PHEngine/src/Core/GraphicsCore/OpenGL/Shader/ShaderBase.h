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
			
			ShaderBase(std::string&& shaderName, std::string&& vertexShaderFile, std::string&& fragmentShaderFile, std::string&& geometryShaderFile = "");

			~ShaderBase();

		protected:

			virtual void AccessAllUniformLocations() override;

			virtual void SetShaderPredefine() override;
		};

	}
}
