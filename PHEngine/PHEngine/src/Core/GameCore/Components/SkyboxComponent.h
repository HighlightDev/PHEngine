#pragma once
#include "PrimitiveComponent.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"
#include "Core/GameCore/ShaderImplementation/SkyboxShader.h"
#include "Core/GraphicsCore/RenderData/SkyboxRenderData.h"

#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Game::ShaderImpl;
using namespace Graphics::Data;

namespace Game
{

	class SkyboxComponent :
		public PrimitiveComponent
	{
	private:

		const float SkyboxSize = 500.0f;

		float m_rotateSpeed;

      SkyboxRenderData m_renderData;

      // MistComponent

	protected:

		using Base = PrimitiveComponent;

	public:

		SkyboxComponent(float rotateSpeed, const SkyboxRenderData& renderData);

		virtual ~SkyboxComponent();

		virtual void Tick(float deltaTime) override;

      virtual uint64_t GetComponentType() const override;

      virtual std::shared_ptr<PrimitiveSceneProxy> CreateSceneProxy() const override;

		inline void SetRotateSpeed(float rotateSpeed)
		{
			m_rotateSpeed = rotateSpeed;
		}

		inline float GetRotateSpeed()
		{
			return m_rotateSpeed;
		}

      inline const SkyboxRenderData& GetRenderData() const {

         return m_renderData;
      }
	};

}

