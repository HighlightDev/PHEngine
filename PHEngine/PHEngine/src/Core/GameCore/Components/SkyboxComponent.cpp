#include "SkyboxComponent.h"
#include "Core/UtilityCore/EngineMath.h"

#include <glm/vec3.hpp>

namespace Game
{
	using namespace EngineUtility;

	SkyboxComponent::SkyboxComponent(std::shared_ptr<Skin> skyboxMesh, std::shared_ptr<ShaderBase> skyboxShader, std::shared_ptr<ITexture> dayTexture, std::shared_ptr<ITexture> nightTexture, float rotateSpeed)
		: PrimitiveComponent(glm::vec3(), glm::vec3(), glm::vec3(1), skyboxMesh, skyboxShader, dayTexture, nightTexture)
		, m_rotateSpeed(rotateSpeed)
		, m_bPostConstructor(true)
		, m_moveFactor(0.0f)
	{

	}

	SkyboxComponent::~SkyboxComponent()
	{

	}

	void SkyboxComponent::PostConstructor()
	{
		if (m_bPostConstructor)
		{

			m_bPostConstructor = !m_bPostConstructor;
		}
	}

	void SkyboxComponent::Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		PostConstructor();

		glDisable(GL_CLIP_DISTANCE0);

		glm::mat4 worldMatrix(1);
		worldMatrix = glm::rotate(worldMatrix, DEG_TO_RAD(m_moveFactor), glm::vec3(0, 1, 0));

		CastToSkyboxShader()->ExecuteShader();

		if (GetDayTexture())
		{
			GetDayTexture()->BindTexture(0);
		}
		if (GetNightTexture())
		{
			GetNightTexture()->BindTexture(1);
		}
		CastToSkyboxShader()->SetTransformMatrices(worldMatrix, viewMatrix, projectionMatrix);
		CastToSkyboxShader()->SetTextures(0, 1);
		//CastToSkyboxShader()->SetDayCycleValue(sunDirection.Normalized().Y);
		//CastToSkyboxShader()->SetMist(m_mist);
		m_skin->GetBuffer()->RenderVAO(GL_TRIANGLES);
		CastToSkyboxShader()->StopShader();
	}

	void SkyboxComponent::Tick(float deltaTime)
	{
		SceneComponent::Tick(deltaTime);
		m_moveFactor += deltaTime * m_rotateSpeed;

		if (m_moveFactor > 360.0f)
		{
			m_moveFactor -= 360.0f;
		}
	}
}