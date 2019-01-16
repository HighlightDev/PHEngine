#include "Engine.h"
#include <GL/glew.h>
#include <iostream>
#include <memory>

#include "Core/GraphicsCore/OpenGL/VertexBufferObject.h"
#include "Core/GraphicsCore/OpenGL/DataCarryFlag.h"
#include "Core/GraphicsCore/OpenGL/Shader/Shader.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/ResourceManagerCore/Pool/PoolBase.h"
#include <Core/ResourceManagerCore/Policy/MeshAllocationPolicy.h>

#include <glm\vec2.hpp>

struct S : public Graphics::OpenGL::Shader
{
	S() : Shader("TestShader", Common::FolderManager::GetInstance()->GetShadersPath() + "testVS.glsl",
		Common::FolderManager::GetInstance()->GetShadersPath() + "testFS.glsl")
	{
		Init();
	}

	virtual void AccessAllUniformLocations()  override
	{

	}

	virtual void SetShaderPredefine() override
	{
		Predefine<glm::vec2>((Graphics::OpenGL::ShaderType)(Graphics::OpenGL::ShaderType::VertexShader | Graphics::OpenGL::ShaderType::GeometryShader), "hallo", glm::vec2(0.5f, 1));
		Predefine<glm::vec2>(Graphics::OpenGL::ShaderType::VertexShader, "hallo", glm::vec2(0.5f, 1));
		Predefine<glm::vec2>(Graphics::OpenGL::ShaderType::VertexShader, "hallo", glm::vec2(0.5f, 1));
	}
};

Engine::Engine()
{
	S shader;

	Resources::PoolBase<Graphics::OpenGL::VertexArrayObject, std::string, Resources::MeshAllocationPolicy> pool;
	std::string pathToFile = Common::FolderManager::GetInstance()->GetModelPath() + "playerCube.obj";
	std::shared_ptr<Graphics::OpenGL::VertexArrayObject> vao = pool.GetOrAllocateResource(pathToFile);
}


Engine::~Engine()
{
}

void Engine::TickWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(1, 0, 0, 0);

	/* Render here */
}
