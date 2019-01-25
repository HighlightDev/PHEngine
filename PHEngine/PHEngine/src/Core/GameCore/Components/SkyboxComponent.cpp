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

		GetSkyboxShader()->ExecuteShader();

		if (GetDayTexture())
		{
			GetDayTexture()->BindTexture(0);
		}
		if (GetNightTexture())
		{
			GetNightTexture()->BindTexture(1);
		}
		GetSkyboxShader()->SetTransformMatrices(m_relativeMatrix, viewMatrix, projectionMatrix);
		GetSkyboxShader()->SetTextures(0, 1);
		//CastToSkyboxShader()->SetDayCycleValue(sunDirection.Normalized().Y);
		//CastToSkyboxShader()->SetMist(m_mist);
		m_skin->GetBuffer()->RenderVAO(GL_TRIANGLES);
		GetSkyboxShader()->StopShader();
	}

	void SkyboxComponent::Tick(float deltaTime)
	{
		Base::Tick(deltaTime);
		
		SetRotationAxisY(m_rotation.y + deltaTime * m_rotateSpeed);
	}
}