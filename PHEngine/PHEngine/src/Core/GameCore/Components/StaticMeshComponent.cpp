#include "StaticMeshComponent.h"

namespace Game
{

	StaticMeshComponent::StaticMeshComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale,
		std::shared_ptr<Skin> skin, std::shared_ptr<ShaderBase> shader, std::shared_ptr<ITexture> albedoTex, std::shared_ptr<ITexture> normalMapTex,
		std::shared_ptr<ITexture> specularMapTex)
		: PrimitiveComponent(std::move(translation), std::move(rotation), std::move(scale), skin, shader, albedoTex, normalMapTex, specularMapTex)
	{

	}

	StaticMeshComponent::~StaticMeshComponent()
	{

	}

	void StaticMeshComponent::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		/*if (clipPlane.X == 0 && clipPlane.Y == 0 && clipPlane.Z == 0 && clipPlane.W == 0) { GL.Disable(EnableCap.ClipDistance0); }
		else { GL.Enable(EnableCap.ClipDistance0); }*/

		GetStaticMeshShader()->ExecuteShader();

		if (m_albedoTex) m_albedoTex->BindTexture(0);
		if (m_normalMapTex) m_normalMapTex->BindTexture(1);
		if (m_specularTex) m_specularTex->BindTexture(2);

		GetStaticMeshShader()->SetAlbedoTex(0);
		GetStaticMeshShader()->SetNormalMapTex(1);
		GetStaticMeshShader()->SetSpecularMapTex(2);
		//GetStaticMeshShader()->SetMaterial(m_material);
		GetStaticMeshShader()->SetTransformationMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);
		//GetStaticMeshShader()->SetPointLights(GetRelevantPointLights(lights));
		//GetStaticMeshShader()->SetDirectionalLight(directionalLight);
		//GetStaticMeshShader()->SetClippingPlane(ref clipPlane);
		//GetStaticMeshShader()->SetMist(m_mist);

		//if (directionalLight != null && directionalLight.GetHasShadow())
		//{
		//	DirectionalLightWithShadow lightWithShadow = directionalLight as DirectionalLightWithShadow;
		//	ITexture shadowMap = lightWithShadow.GetShadowMapTexture();
		//	shadowMap.BindTexture(TextureUnit.Texture4); // shadowmap
		//	GetShader().SetDirectionalLightShadowMatrix(lightWithShadow.GetShadowMatrix(ref modelMatrix, ref ProjectionMatrix));
		//}
		//GetShader().SetDirectionalLightShadowMap(4);

		if (m_skin) m_skin->GetBuffer()->RenderVAO(GL_TRIANGLES);
		GetStaticMeshShader()->StopShader();
	}

	void StaticMeshComponent::Tick(float deltaTime)
	{
		Base::Tick(deltaTime);

		m_rotation += 0.02f;

		if (m_rotation.x > 360.0f)
			m_rotation.x -= 360.0f;

		if (m_rotation.y > 360.0f)
			m_rotation.y -= 360.0f;

		if (m_rotation.z > 360.0f)
			m_rotation.z -= 360.0f;

		SetRotation(m_rotation);
	}

}
