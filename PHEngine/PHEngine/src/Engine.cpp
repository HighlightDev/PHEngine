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

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;
using namespace Game::Components;

Engine::Engine()
{
	// MATRICES
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 eye = glm::vec3(0, 0, -10);
		glm::vec3 center = glm::vec3(0, 0, 0);

		m_viewMatrix = glm::lookAt(eye, center, up);

		const float aspectRatio = 16.0f / 9.0f;
		projectionMatrix = glm::perspective<float>(DEG_TO_RAD(60), aspectRatio, 1, 100);
	}

	// RESOURCES
	{
		// MESH
		std::string pathToFile = FolderManager::GetInstance()->GetModelPath() + "City_House_2_BI.obj";
		m_skin = m_meshPool.GetOrAllocateResource(pathToFile);
		// TEXTURE
		std::string pathToTexture = FolderManager::GetInstance()->GetLandscapeTexturePath() + "b.png";
		m_texture = m_texturePool.GetOrAllocateResource(pathToTexture);
	}

	float SKYBOX_SIZE = 50.0f;

	std::vector<float> vertices = { -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE , SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE ,
				 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE , SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE ,
				 -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE ,
				 -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE , SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE , SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE , SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE ,
				 SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE , SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE ,
				 -SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE , SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE , SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE , SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE ,
				 -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE , -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE ,
				 SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE , -SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE , -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE ,
				 -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE , SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE , SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE , -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE ,
				 SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE };

	auto vertPtr = std::make_shared<std::vector<float>>(vertices);

	std::unique_ptr<VertexArrayObject> vao = std::make_unique<VertexArrayObject>();
	VertexBufferObjectBase* vbo = new VertexBufferObject<float, 3, GL_FLOAT>(vertPtr, GL_ARRAY_BUFFER, 0, DataCarryFlag::Invalidate);
	vao->AddVBO(std::unique_ptr<VertexBufferObjectBase>(vbo));

	vao->BindBuffersToVao();

	auto skin = std::make_shared<Skin>(std::move(vao));

	std::shared_ptr<SkyboxShader> skyShader = std::make_shared<SkyboxShader>(FolderManager::GetInstance()->GetShadersPath() + "tSkyboxVS.glsl",
		FolderManager::GetInstance()->GetShadersPath() + "tSkyboxFS.glsl");


	StringStreamWrapper::ToString(FolderManager::GetInstance()->GetCubemapTexturePath(),"Day/" ,"right.bmp", ",",
		FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "left.bmp", ",",
		FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "top.bmp", ",",
		FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "bottom.bmp", ",",
		FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "back.bmp", ",",
		FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "front.bmp");

	auto dTexPath = StringStreamWrapper::FlushString();
	auto dTex = m_texturePool.GetOrAllocateResource(dTexPath);
	auto nTex = std::shared_ptr<ITexture>();
	std::shared_ptr<Component> skyboxComponent = std::make_shared<SkyboxComponent>(skin, skyShader, dTex, nTex , 0.0f);

	Actor skybox;
	skybox.AddComponent(skyboxComponent);
	m_allActors.emplace_back(std::move(skybox));
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