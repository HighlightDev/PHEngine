#include "SkyboxShader.h"

namespace Game
{
	namespace ShaderImpl
	{

		SkyboxShader::SkyboxShader(std::string&& vsPath, std::string&& fsPath)
			: ShaderBase("Skybox Shader", std::move(vsPath), std::move(fsPath))
		{
		}


		SkyboxShader::~SkyboxShader()
		{
		}

		void SkyboxShader::SetTransformMatrices(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
		{
			u_worldMatrix.LoadUniform(worldMatrix);
			u_viewMatrix.LoadUniform(viewMatrix);
			u_projectionMatrix.LoadUniform(projectionMatrix);
		}

		void SkyboxShader::SetTextures(int32_t dayTexSlot, int32_t nightTexSlot)
		{
			u_dayTexture.LoadUniform(dayTexSlot);
			u_nightTexture.LoadUniform(nightTexSlot);
		}

		void SkyboxShader::AccessAllUniformLocations()
		{
			u_worldMatrix = GetUniform("worldMatrix");
			u_viewMatrix = GetUniform("viewMatrix");
			u_projectionMatrix = GetUniform("projectionMatrix");
			u_dayTexture = GetUniform("dayTexture");
			u_nightTexture = GetUniform("nightTexture");
		}

		void SkyboxShader::SetShaderPredefine()
		{

		}

	}
}