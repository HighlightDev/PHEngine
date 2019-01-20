#pragma once

#include <gl/glew.h>
#include <memory>

#include "Core/GraphicsCore/OpenGL/VertexArrayObject.h"
#include "Core/GraphicsCore/OpenGL/VertexBufferObject.h"
#include "Core/GraphicsCore/OpenGL/DataCarryFlag.h"
#include "Core/GraphicsCore/OpenGL/Shader/Shader.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/ResourceManagerCore/Pool/PoolBase.h"
#include "Core/ResourceManagerCore/Policy/MeshAllocationPolicy.h"
#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/Texture/Texture2d.h"
#include "Core/GraphicsCore/Texture/TextureMipMapState.h"


struct BasicShader : public Graphics::OpenGL::Shader
{

	Uniform m_viewMatrix, m_projectionMatrix, m_texture;

	BasicShader() : Shader("TestShader", Common::FolderManager::GetInstance()->GetShadersPath() + "testVS.glsl",
		Common::FolderManager::GetInstance()->GetShadersPath() + "testFS.glsl")
	{
		Init();
	}

	virtual void AccessAllUniformLocations()  override
	{
		m_viewMatrix = GetUniform("viewMatrix");
		m_projectionMatrix = GetUniform("projectionMatrix");
		m_texture = GetUniform("albedo");
	}

	void SetTransformationMatrices(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		m_viewMatrix.LoadUniform(viewMatrix);
		m_projectionMatrix.LoadUniform(projectionMatrix);
	}

	void SetTexSampler(int32_t slot)
	{
		m_texture.LoadUniform(slot);
	}

	virtual void SetShaderPredefine() override
	{
		Predefine<glm::vec2>((Graphics::OpenGL::ShaderType)(Graphics::OpenGL::ShaderType::VertexShader | Graphics::OpenGL::ShaderType::GeometryShader), "hallo", glm::vec2(0.5f, 1));
	}
};

class Engine
{
	BasicShader m_shader;
	glm::mat4 m_viewMatrix;
	glm::mat4 projectionMatrix;
	Resources::PoolBase<Graphics::Mesh::Skin, std::string, Resources::MeshAllocationPolicy> pool;
	Graphics::Texture::Texture2d* m_texture;
	std::shared_ptr<Graphics::Mesh::Skin> m_skin;

	float* vertices;

	uint32_t vbo;

	bool bPreconstructor = false;

public:
	Engine();
	~Engine();

	void TickWindow();
};

