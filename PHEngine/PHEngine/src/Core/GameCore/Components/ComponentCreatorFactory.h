#pragma once

#include <memory>

#include "Component.h"
#include "SkyboxComponent.h"
#include "ComponentType.h"
#include "Core/ResourceManagerCore/Pool/TexturePool.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GameCore/Components/ComponentData/SkyboxComponentData.h"

using namespace Resources;

namespace Game
{
	namespace Components
	{

		template <typename T>
		struct ComponentCreatorFactory;

		// Skybox component
		template <>
		struct ComponentCreatorFactory<SkyboxComponent>
		{
			static std::shared_ptr<Component> CreateComponent(ComponentData& data)
			{
				std::shared_ptr<Component> resultComponent;
				if (data.GetType() == ComponentType::SKYBOX_COMPONENT)
				{
					SkyboxComponentData& mData = static_cast<SkyboxComponentData&>(data);

					const float skyboxSize = mData.m_skyboxSideSize;
					

					std::vector<float> vertices = { -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, -skyboxSize ,
						skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, skyboxSize ,
						-skyboxSize, -skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, skyboxSize ,
						-skyboxSize, -skyboxSize, skyboxSize , skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, skyboxSize , skyboxSize, skyboxSize, skyboxSize ,
						skyboxSize, skyboxSize, skyboxSize , skyboxSize, skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, -skyboxSize, -skyboxSize, -skyboxSize, skyboxSize ,
						-skyboxSize, skyboxSize, skyboxSize , skyboxSize, skyboxSize, skyboxSize , skyboxSize, skyboxSize, skyboxSize , skyboxSize, -skyboxSize, skyboxSize ,
						-skyboxSize, -skyboxSize, skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , skyboxSize, skyboxSize, -skyboxSize , skyboxSize, skyboxSize, skyboxSize ,
						skyboxSize, skyboxSize, skyboxSize , -skyboxSize, skyboxSize, skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, -skyboxSize ,
						-skyboxSize, -skyboxSize, skyboxSize , skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, skyboxSize ,
						skyboxSize, -skyboxSize, skyboxSize };

					std::unique_ptr<VertexArrayObject> vao = std::make_unique<VertexArrayObject>();
					VertexBufferObjectBase* vbo = new VertexBufferObject<float, 3, GL_FLOAT>(std::make_shared<std::vector<float>>(vertices), GL_ARRAY_BUFFER, 0, DataCarryFlag::Invalidate);
					vao->AddVBO(std::unique_ptr<VertexBufferObjectBase>(std::move(vbo)));

					vao->BindBuffersToVao();

					auto skin = std::make_shared<Skin>(std::move(vao));

					TexturePool::tDataType dTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesDay);
					TexturePool::tDataType nTex;
					if (mData.m_commaSeparatedPathToSixTexturesNight != "")
						nTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesNight);

					std::shared_ptr<SkyboxShader> skyShader = std::make_shared<SkyboxShader>(std::move(mData.m_vsShaderPath), std::move(mData.m_fsShaderPath));

					resultComponent = std::make_shared<SkyboxComponent>(skin, skyShader, dTex, nTex, 0.0f);
				}

				return resultComponent;
			}
		};

	}
}