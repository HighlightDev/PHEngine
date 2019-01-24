#include "Engine.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Core/IoCore/TextureLoaderCore/StbLoader/StbLoader.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/UtilityCore/EngineMath.h"
#include "Core/GraphicsCore/Texture/TexParams.h"
#include "Core/GameCore/Components/SkyboxComponent.h"
#include "Core/GameCore/Components/ComponentCreatorFactory.h"
#include "Core/ResourceManagerCore/Pool/MeshPool.h"
#include "Core/ResourceManagerCore/Pool/TexturePool.h"
#include "Core/GameCore/GlobalProperties.h"

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;
using namespace Game;

Engine::Engine()
	: camera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10))
{
		const float aspectRatio = 16.0f / 9.0f;
		projectionMatrix = glm::perspective<float>(DEG_TO_RAD(60), aspectRatio, 1, 100);

	// RESOURCES
	{
		// MESH
		std::string pathToFile = FolderManager::GetInstance()->GetModelPath() + "City_House_2_BI.obj";
		m_skin = MeshPool::GetInstance()->GetOrAllocateResource(pathToFile);
		// TEXTURE
		std::string pathToTexture = FolderManager::GetInstance()->GetAlbedoTexturePath() + "city_house_2_Col.png";
		m_texture = TexturePool::GetInstance()->GetOrAllocateResource(pathToTexture);
	}

	// SKYBOX
	{
		float SKYBOX_SIZE = 50.0f;

		StringStreamWrapper::ToString(
			FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "right.png", ",",
			FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "left.png", ",",
			FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "top.png", ",",
			FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "bottom.png", ",",
			FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "back.png", ",",
			FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "front.png");
		auto dTexPath = StringStreamWrapper::FlushString();

		SkyboxComponentData mData(SKYBOX_SIZE, std::move(FolderManager::GetInstance()->GetShadersPath() + "tSkyboxVS.glsl"),
			std::move(FolderManager::GetInstance()->GetShadersPath() + "tSkyboxFS.glsl"), std::move(dTexPath));

		std::shared_ptr<Component> skyboxComp = ComponentCreatorFactory<SkyboxComponent>::CreateComponent(mData);

		Actor skyboxActor = Actor(new SceneComponent(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
		skyboxActor.AddComponent(skyboxComp);
		m_allActors.emplace_back(std::move(skyboxActor));
	}
}

Engine::~Engine()
{
}

void Engine::UpdateWorldMatrix()
{
	float angleRad = DEG_TO_RAD(rotateAngle);

	worldMatrix = glm::mat4(1);
	worldMatrix = glm::scale(worldMatrix, glm::vec3(1));
	worldMatrix = glm::rotate(worldMatrix, angleRad, glm::vec3(1, 0, 0));
	worldMatrix = glm::rotate(worldMatrix, angleRad, glm::vec3(0, 1, 0));
	worldMatrix = glm::rotate(worldMatrix, angleRad, glm::vec3(0, 0, 1));
	worldMatrix = glm::translate(worldMatrix, glm::vec3(0));

	rotateAngle += 0.02f;
	if (rotateAngle > 360.0f)
	{
		rotateAngle -= 360.0f;
	}

	m_viewMatrix = camera.GetViewMatrix();
}

void Engine::TickWindow()
{
	// BUILD WORLD MATRIX 
	UpdateWorldMatrix();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	m_shader.ExecuteShader();
	m_texture->BindTexture(0);
	m_shader.SetTexSampler(0);
	m_shader.SetTransformationMatrices(worldMatrix, m_viewMatrix, projectionMatrix);
	m_skin->GetBuffer()->RenderVAO(GL_TRIANGLES);
	m_shader.StopShader();

	for (auto it = m_allActors.begin(); it != m_allActors.end(); ++it)
	{
		it->Render(m_viewMatrix, projectionMatrix, 0.0f);
	}

}

void Engine::MouseMove()
{
	camera.Rotate();
}