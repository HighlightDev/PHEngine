#include "ResolveTextureShader.h"

namespace Game
{
	namespace ShaderImpl
	{

		ResolveTextureShader::ResolveTextureShader(std::string&& vsPath, std::string&& fsPath)
			: ShaderBase("Resolve texture shader;", std::move(vsPath), std::move(fsPath))
		{
		}

		ResolveTextureShader::~ResolveTextureShader()
		{
		}

		void ResolveTextureShader::AccessAllUniformLocations()
		{
			Base::AccessAllUniformLocations();

			u_textureSampler = GetUniform("textureSampler");
		}

		void ResolveTextureShader::SetShaderPredefine() 
		{

		}

		void ResolveTextureShader::SetTextureShader(int32_t slot)
		{
			u_textureSampler.LoadUniform(slot);
		}
	}
}
