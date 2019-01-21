#pragma once
#include "Component.h"
#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

#include <glm/mat4x4.hpp>
#include <memory>

using namespace Graphics::Mesh;
using namespace Graphics::Texture;

namespace Game
{
	namespace Components
	{
		// Base class of all renderable components
		class SceneComponent :
			public Component
		{
		protected:

			std::shared_ptr<Skin> m_skin;

			std::shared_ptr<ShaderBase> m_shader;

			std::shared_ptr<ITexture> m_albedoTex;
			std::shared_ptr<ITexture> m_normalMapTex;
			std::shared_ptr<ITexture> m_specularTex;

		public:

			SceneComponent(std::shared_ptr<Skin> skin, std::shared_ptr<ShaderBase> shader, std::shared_ptr<ITexture> albedoTex = std::shared_ptr<ITexture>(),
				std::shared_ptr<ITexture> normalMapTex = std::shared_ptr<ITexture>(),
				std::shared_ptr<ITexture> specularMapTex = std::shared_ptr<ITexture>());

			virtual ~SceneComponent();

			virtual ComponentType GetComponentType() override
			{
				return ComponentType::SCENE_COMPONENT;
			}

			virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) = 0;
		};

	}
}

