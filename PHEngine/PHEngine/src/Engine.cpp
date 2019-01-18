#include "Engine.h"
#include <GL/glew.h>
#include <iostream>
#include <memory>

#include "Core/GraphicsCore/OpenGL/VertexBufferObject.h"
#include "Core/GraphicsCore/OpenGL/DataCarryFlag.h"
#include "Core/GraphicsCore/OpenGL/Shader/Shader.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/ResourceManagerCore/Pool/PoolBase.h"
#include "Core/ResourceManagerCore/Policy/MeshAllocationPolicy.h"
#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/Texture/Texture2d.h"
#include "Core/GraphicsCore/Texture/TextureMipMapState.h"

#include <glm/vec2.hpp>

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
	}
};

Engine::Engine()
{
	S shader;

	Resources::PoolBase<Graphics::Mesh::Skin, std::string, Resources::MeshAllocationPolicy> pool;
	std::string pathToFile = Common::FolderManager::GetInstance()->GetModelPath() + "playerCube.obj";
	std::shared_ptr<Graphics::Mesh::Skin> skin = pool.GetOrAllocateResource(pathToFile);

	pool.TryToFreeMemory(skin);

	std::string pathToTexture = Common::FolderManager::GetInstance()->GetModelPath() + "playerCube.obj";
	
	Graphics::Texture::TextureMipMap mipMap;
	Graphics::Texture::Texture2d* tex = new Graphics::Texture::Texture2d(pathToTexture, mipMap);
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
