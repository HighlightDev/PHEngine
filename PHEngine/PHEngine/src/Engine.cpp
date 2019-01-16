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
	std::vector<float> v1 = { 1, 2, 3 };
	std::shared_ptr<std::vector<float>> s1 = std::make_shared<std::vector<float>>(v1);

	Graphics::OpenGL::VertexBufferObjectBase* vbo = new Graphics::OpenGL::VertexBufferObject<float, 3, GL_FLOAT>(s1, GL_ARRAY_BUFFER, 0, Graphics::OpenGL::DataCarryFlag::Invalidate);
	Graphics::OpenGL::VertexBufferObjectBase* vbo1 = new Graphics::OpenGL::VertexBufferObject<float, 3, GL_FLOAT>(s1, GL_ARRAY_BUFFER, 1, Graphics::OpenGL::DataCarryFlag::Invalidate);
	Graphics::OpenGL::VertexBufferObjectBase* vbo2 = new Graphics::OpenGL::VertexBufferObject<float, 3, GL_FLOAT>(s1, GL_ARRAY_BUFFER, 2, Graphics::OpenGL::DataCarryFlag::Invalidate);
	Graphics::OpenGL::VertexBufferObjectBase* vbo3 = new Graphics::OpenGL::VertexBufferObject<float, 3, GL_FLOAT>(s1, GL_ARRAY_BUFFER, 3, Graphics::OpenGL::DataCarryFlag::Invalidate);
		
	Graphics::OpenGL::VertexArrayObject vao;

	std::unique_ptr<Graphics::OpenGL::VertexBufferObjectBase> p_vbo(vbo);
	std::unique_ptr<Graphics::OpenGL::VertexBufferObjectBase> p_vbo1(vbo1);
	std::unique_ptr<Graphics::OpenGL::VertexBufferObjectBase> p_vbo2(vbo2);
	std::unique_ptr<Graphics::OpenGL::VertexBufferObjectBase> p_vbo3(vbo3);

	vao.AddVBO(std::move(p_vbo), std::move(p_vbo1), std::move(p_vbo2), std::move(p_vbo3));
	vao.GenVAO();
	vao.BindBuffersToVao();

	Resources::PoolBase<int32_t, std::string, Resources::MeshAllocationPolicy> pool;
	std::string pathToFile = Common::FolderManager::GetInstance()->GetShadersPath() + "testVS.glsl";
	pool.GetOrAllocateResource(pathToFile);
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
