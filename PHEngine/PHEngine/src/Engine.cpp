#include "Engine.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Core/IoCore/TextureLoaderCore/StbLoader/StbLoader.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/GraphicsCore/Texture/TexParams.h"

#define PI 3.14f
#define DEG_TO_RAD(X) (X * (PI / 180.0f))

using namespace Graphics::Texture;
using namespace Common;

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
		std::string pathToFile = FolderManager::GetInstance()->GetModelPath() + "playerCube.obj";
		m_skin = m_meshPool.GetOrAllocateResource(pathToFile);
		// TEXTURE
		std::string pathToTexture = FolderManager::GetInstance()->GetLandscapeTexturePath() + "b.png";
		m_texture = m_texturePool.GetOrAllocateResource(pathToTexture);
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

	rotateAngle += 0.05f;
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

}