#pragma once
#include "PrimitiveComponent.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"
#include "Core/GameCore/ShaderImplementation/SkyboxShader.h"

#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Game::ShaderImpl;

namespace Game
{

	class SkyboxComponent :
		public PrimitiveComponent
	{
	private:

		const float SkyboxSize = 500.0f;

		float m_rotateSpeed;

		bool m_bPostConstructor;

		// MistComponent

	protected:

		using Base = PrimitiveComponent;

	public:

		SkyboxComponent(std::shared_ptr<Skin> skyboxMesh, std::shared_ptr<ShaderBase> skyboxShader, std::shared_ptr<ITexture> dayTexture, std::shared_ptr<ITexture> nightTexture, float rotateSpeed);

		virtual ~SkyboxComponent();

		virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

		virtual void Tick(float deltaTime) override;

		virtual ComponentType GetComponentType() override
		{
			return ComponentType::SCENE_COMPONENT;
		}

		inline void SetRotateSpeed(float rotateSpeed)
		{
			m_rotateSpeed = rotateSpeed;
		}

		inline float GetRotateSpeed()
		{
			return m_rotateSpeed;
		}

	private:

		inline SkyboxShader* GetSkyboxShader() const
		{
			SkyboxShader* shader = static_cast<SkyboxShader*>(m_shader.get());
			return shader;
		}

		inline std::shared_ptr<ITexture> GetDayTexture()
		{
			return m_albedoTex;
		}

		inline std::shared_ptr<ITexture> GetNightTexture()
		{
			return m_normalMapTex;
		}

		void PostConstructor();
	};

}

