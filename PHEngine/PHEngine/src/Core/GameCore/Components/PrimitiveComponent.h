#pragma once
#include "SceneComponent.h"
#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>

using namespace Graphics::Mesh;
using namespace Graphics::Texture;

namespace Game
{
	// Base class of all renderable components
	class PrimitiveComponent :
		public SceneComponent
	{
	protected:

		using Base = SceneComponent;

		std::shared_ptr<Skin> m_skin;

		std::shared_ptr<ShaderBase> m_shader;

		std::shared_ptr<ITexture> m_albedoTex;
		std::shared_ptr<ITexture> m_normalMapTex;
		std::shared_ptr<ITexture> m_specularTex;

	public:

		PrimitiveComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, std::shared_ptr<Skin> skin, std::shared_ptr<ShaderBase> shader, std::shared_ptr<ITexture> albedoTex = std::shared_ptr<ITexture>(),
			std::shared_ptr<ITexture> normalMapTex = std::shared_ptr<ITexture>(),
			std::shared_ptr<ITexture> specularMapTex = std::shared_ptr<ITexture>());

		virtual ~PrimitiveComponent();

		virtual ComponentType GetComponentType() override
		{
			return ComponentType::PRIMITIVE_COMPONENT;
		}

		virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) = 0;
	};

}

