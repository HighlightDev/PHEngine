#include "SceneComponent.h"

namespace Game
{
	namespace Components
	{

		SceneComponent::SceneComponent(std::shared_ptr<Skin> skin, std::shared_ptr<ShaderBase> shader, std::shared_ptr<ITexture> albedoTex, std::shared_ptr<ITexture> normalMapTex,
			std::shared_ptr<ITexture> specularMapTex)
			: Component()
			, m_skin(skin)
			, m_shader(shader)
			, m_albedoTex(albedoTex)
			, m_normalMapTex(normalMapTex)
			, m_specularTex(specularMapTex)
		{
		}

		SceneComponent::~SceneComponent()
		{
		}
	}
}