#include "Engine.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.14f
#define DEG_TO_RAD(X) (X * (PI / 180.0f))

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::TickWindow()
{
	if (!bPreconstructor)
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 eye = glm::vec3(0, 0, -1);
		glm::vec3 center = glm::vec3(0, 0, 0);

		m_viewMatrix = glm::lookAt(eye, center, up);
		projectionMatrix = glm::perspective<float>(DEG_TO_RAD(75), (9.0f / 16.0f), 1, 100);

		std::string pathToFile = Common::FolderManager::GetInstance()->GetModelPath() + "playerCube.obj";
		m_skin = pool.GetOrAllocateResource(pathToFile);

		Graphics::Texture::TextureAnisotropy anisotropy = Graphics::Texture::TextureAnisotropy(8.0f);
		std::string pathToTexture = Common::FolderManager::GetInstance()->GetAlbedoTexturePath() + "diffuse.png";
		m_texture = new Graphics::Texture::Texture2d(pathToTexture, anisotropy);

		vertices = new float[9]{ -0.5, 0, 0, 0.5, 0 , 0 ,0, 0.5, 0 };

		const size_t size = sizeof(float) * 9;

		glGenBuffers(1, &vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(1, 0, 0, 0);


	m_shader.ExecuteShader();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_shader.StopShader();


	/*m_shader.ExecuteShader();
	m_texture->BindTexture(0);
	m_shader.SetTransformationMatrices(m_viewMatrix, projectionMatrix);
	m_shader.SetTexSampler(0);
	m_skin->GetBuffer()->RenderVAO();
	m_shader.StopShader();*/


	/* Render here */
}
