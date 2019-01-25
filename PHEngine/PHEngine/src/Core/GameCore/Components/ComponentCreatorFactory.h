#pragma once

#include <memory>

#include "Component.h"
#include "SkyboxComponent.h"
#include "Core/GameCore/Components/StaticMeshComponent.h"
#include "ComponentType.h"
#include "Core/ResourceManagerCore/Pool/TexturePool.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"
#include "Core/ResourceManagerCore/Pool/MeshPool.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GameCore/Components/ComponentData/SkyboxComponentData.h"
#include "Core/GameCore/Components/ComponentData/StaticMeshComponentData.h"

using namespace Resources;

namespace Game
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

					typename TexturePool::sharedValue_t dTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesDay);
					typename TexturePool::sharedValue_t nTex;
					if (mData.m_commaSeparatedPathToSixTexturesNight != "")
						nTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesNight);

					std::string shaderPath = std::move(mData.m_vsShaderPath) + "," + std::move(mData.m_fsShaderPath);
					ShaderPool::sharedValue_t skyboxShader = ShaderPool::GetInstance()->template GetOrAllocateResource<SkyboxShader>(shaderPath);

					resultComponent = std::make_shared<SkyboxComponent>(skin, skyboxShader, dTex, nTex, mData.m_rotateSpeed);
				}

				return resultComponent;
			}
		};

		//Static Mesh component
		template <>
		struct ComponentCreatorFactory<StaticMeshComponent>
		{
			static std::shared_ptr<Component> CreateComponent(ComponentData& data)
			{
				std::shared_ptr<Component> resultComponent;
				if (data.GetType() == ComponentType::STATIC_MESH_COMPONENT)
				{
					StaticMeshComponentData& mData = static_cast<StaticMeshComponentData&>(data);

					typename MeshPool::sharedValue_t skin = MeshPool::GetInstance()->GetOrAllocateResource(mData.m_pathToMesh);
				
					typename TexturePool::sharedValue_t albedo = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToAlbedo);
					typename TexturePool::sharedValue_t normalMap;
					typename TexturePool::sharedValue_t specularMap;

					if (mData.m_pathToNormalMap != "")
						normalMap = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToNormalMap);

					if (mData.m_pathToSpecularMap != "")
						specularMap = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToSpecularMap);

					std::string shaderPath = std::move(mData.m_vsShaderPath) + "," + std::move(mData.m_fsShaderPath);
					ShaderPool::sharedValue_t staticMeshShader = ShaderPool::GetInstance()->template GetOrAllocateResource<StaticMeshShader>(shaderPath);

					resultComponent = std::make_shared<StaticMeshComponent>(std::move(mData.m_translation), std::move(mData.m_rotation), std::move(mData.m_scale),
						skin, staticMeshShader, albedo, normalMap, specularMap);
				}

				return resultComponent;
			}
		};

}