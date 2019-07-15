#include "SkyboxShader.h"

namespace Game
{
	namespace ShaderImpl
	{

		SkyboxShader::SkyboxShader(const ShaderParams& params)
			: ShaderBase(params)
		{
         ShaderInit();
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
			ShaderBase::AccessAllUniformLocations();

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