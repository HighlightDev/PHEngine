#include "StaticMeshShader.h"


namespace Game
{
	namespace ShaderImpl
	{

		StaticMeshShader::StaticMeshShader(const ShaderParams& params)
			: ShaderBase(params)
		{
			Init();
		}

		StaticMeshShader::~StaticMeshShader()
		{

		}

		void StaticMeshShader::SetTransformationMatrices(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
		{
			u_worldMatrix.LoadUniform(worldMatrix);
			u_viewMatrix.LoadUniform(viewMatrix);
			u_projectionMatrix.LoadUniform(projectionMatrix);
		}

		void StaticMeshShader::SetAlbedoTex(int32_t slot) {

			u_albedo.LoadUniform(slot);
		}

		void StaticMeshShader::SetNormalMapTex(int32_t slot) {

			u_normalMap.LoadUniform(slot);
		}

		void StaticMeshShader::SetSpecularMapTex(int32_t slot) {

			u_specularMap.LoadUniform(slot);
		}

		void StaticMeshShader::AccessAllUniformLocations()
		{
         Base::AccessAllUniformLocations();

			u_worldMatrix = GetUniform("worldMatrix");
			u_viewMatrix = GetUniform("viewMatrix");
			u_projectionMatrix = GetUniform("projectionMatrix");
			u_albedo = GetUniform("albedo");
			u_normalMap = GetUniform("normalMap");
			u_specularMap = GetUniform("specularMap");

			u_materialAmbient = GetUniform("materialAmbient");
			u_materialDiffuse = GetUniform("materialDiffuse");
			u_materialSpecular = GetUniform("materialSpecular");
			u_materialReflectivity = GetUniform("materialReflectivity");
			u_materialShineDamper = GetUniform("materialShineDamper");
			u_sunDirection = GetUniform("sunDirection");
			u_sunAmbientColour = GetUniform("sunAmbientColour");
			u_sunDiffuseColour = GetUniform("sunDiffuseColour");
			u_sunSpecularColour = GetUniform("sunSpecularColour");
			u_sunEnable = GetUniform("sunEnable");
			u_mistEnable = GetUniform("mistEnable");
			u_mistDensity = GetUniform("mistDensity");
			u_mistGradient = GetUniform("mistGradient");
			u_mistColour = GetUniform("mistColour");
			for (size_t i = 0; i < MAX_LIGHT_COUNT; i++)
			{
				/*u_lightPosition[i] = GetUniform("lightPosition[" + i + "]");
				u_attenuation[i] = GetUniform("attenuation[" + i + "]");
				u_diffuseColour[i] = GetUniform("diffuseColour[" + i + "]");
				u_specularColour[i] = GetUniform("specularColour[" + i + "]");
				u_enableLight[i] = GetUniform("enableLight[" + i + "]");*/
			}
			u_clipPlane = GetUniform("clipPlane");
			u_directionalLightShadowMap = GetUniform("dirLightShadowMap");
			u_directionalLightShadowMatrix = GetUniform("dirLightShadowMatrix");
			u_bEnableNormalMap = GetUniform("bEnableNormalMap");
			u_bEnableSpecularMap = GetUniform("bEnableSpecularMap");
		}

		void StaticMeshShader::SetShaderPredefine()
		{
			
		}

	}
}