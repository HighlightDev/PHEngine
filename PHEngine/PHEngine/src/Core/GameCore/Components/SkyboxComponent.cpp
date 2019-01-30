#include "SkyboxComponent.h"
#include "Core/UtilityCore/EngineMath.h"
#include "Core/GraphicsCore/PrimitiveProxy/SkyboxSceneProxy.h"

#include <glm/vec3.hpp>

namespace Game
{
	using namespace EngineUtility;

	SkyboxComponent::SkyboxComponent(float rotateSpeed, SkyboxRenderData& renderData)
		: PrimitiveComponent(glm::vec3(), glm::vec3(), glm::vec3(1))
		, m_rotateSpeed(rotateSpeed)
      , m_renderData(renderData)
	{

	}

	SkyboxComponent::~SkyboxComponent()
	{

	}

	void SkyboxComponent::Tick(float deltaTime)
	{
		Base::Tick(deltaTime);
		
		SetRotationAxisY(m_rotation.y + deltaTime * m_rotateSpeed);
	}

   std::shared_ptr<PrimitiveSceneProxy> SkyboxComponent::CreateSceneProxy()
   {
      return std::make_shared<SkyboxSceneProxy>(m_relativeMatrix, m_renderData.m_skin, m_renderData.m_shader, m_renderData.m_dayTex, m_renderData.m_nightTex);
   }
}