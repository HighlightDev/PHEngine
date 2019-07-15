#include "ResolveTextureShader.h"

namespace Game
{
	namespace ShaderImpl
	{

		ResolveTextureShader::ResolveTextureShader(const ShaderParams& params)
			: ShaderBase(params)
		{
         ShaderInit();
		}

		ResolveTextureShader::~ResolveTextureShader()
		{
		}

		void ResolveTextureShader::AccessAllUniformLocations()
		{
			Base::AccessAllUniformLocations();

			u_textureSampler = GetUniform("SrcColor");
		}

		void ResolveTextureShader::SetShaderPredefine() 
		{

		}

		void ResolveTextureShader::SetTextureSlot(int32_t slot)
		{
			u_textureSampler.LoadUniform(slot);
		}
	}
}
