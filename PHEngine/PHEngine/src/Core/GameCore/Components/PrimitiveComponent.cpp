#include "PrimitiveComponent.h"

namespace Game
{

	PrimitiveComponent::PrimitiveComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale,
		std::shared_ptr<Skin> skin, std::shared_ptr<ShaderBase> shader, std::shared_ptr<ITexture> albedoTex, std::shared_ptr<ITexture> normalMapTex,
		std::shared_ptr<ITexture> specularMapTex)
		: SceneComponent(std::move(translation), std::move(rotation), std::move(scale))
		, m_skin(skin)
		, m_shader(shader)
		, m_albedoTex(albedoTex)
		, m_normalMapTex(normalMapTex)
		, m_specularTex(specularMapTex)
	{
	}

	PrimitiveComponent::~PrimitiveComponent()
	{
	}
}