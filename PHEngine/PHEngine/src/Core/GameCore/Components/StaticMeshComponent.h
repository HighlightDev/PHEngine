#pragma once
#include "PrimitiveComponent.h"
#include "Core/GameCore/ShaderImplementation/StaticMeshShader.h"

#include <glm/mat4x4.hpp>

using namespace Game::ShaderImpl;

namespace Game
{

	class StaticMeshComponent :
		public PrimitiveComponent
	{
	protected:

		using Base = PrimitiveComponent;

	private:

		inline StaticMeshShader* GetStaticMeshShader() const
		{
			StaticMeshShader* shader = static_cast<StaticMeshShader*>(m_shader.get());
			return shader;
		}

	public:

		StaticMeshComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale,
			std::shared_ptr<Skin> skin, std::shared_ptr<ShaderBase> shader, std::shared_ptr<ITexture> albedoTex, std::shared_ptr<ITexture> normalMapTex,
			std::shared_ptr<ITexture> specularMapTex);

		virtual ~StaticMeshComponent();

		virtual ComponentType GetComponentType() override
		{
			return ComponentType::STATIC_MESH_COMPONENT;
		}

		virtual void Tick(float deltaTime) override;

		virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;
	};

}

