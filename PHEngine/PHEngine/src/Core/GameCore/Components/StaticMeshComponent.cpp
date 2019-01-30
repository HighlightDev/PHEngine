#include "StaticMeshComponent.h"
#include "Core/GraphicsCore/PrimitiveProxy/StaticMeshSceneProxy.h"

#include <memory>

namespace Game
{

	StaticMeshComponent::StaticMeshComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, StaticMeshRenderData& renderData)
		: PrimitiveComponent(std::move(translation), std::move(rotation), std::move(scale))
      , m_renderData(renderData)
	{

	}

	StaticMeshComponent::~StaticMeshComponent()
	{

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

   std::shared_ptr<PrimitiveSceneProxy> StaticMeshComponent::CreateSceneProxy()
   {
      return std::make_shared<StaticMeshSceneProxy>(m_relativeMatrix, m_renderData.m_skin,
         m_renderData.m_shader, m_renderData.m_albedo, m_renderData.m_normalMapTex, m_renderData.m_specularMapTex);
   }

}
