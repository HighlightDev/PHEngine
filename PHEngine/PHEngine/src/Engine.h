#pragma once

#include <gl/glew.h>
#include <memory>

#include "Core/GraphicsCore/OpenGL/VertexArrayObject.h"
#include "Core/GraphicsCore/OpenGL/VertexBufferObject.h"
#include "Core/GraphicsCore/OpenGL/DataCarryFlag.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/ResourceManagerCore/Pool/PoolBase.h"
#include "Core/ResourceManagerCore/Policy/MeshAllocationPolicy.h"
#include "Core/ResourceManagerCore/Policy/TextureAllocationPolicy.h"
#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/Texture/Texture2d.h"
#include "Core/GraphicsCore/Texture/TextureMipMapState.h"
#include "Core/GameCore/Actor.h"

using namespace Game;
using namespace Graphics::OpenGL;

struct BasicShader : public ShaderBase
{

	Uniform m_worldMatrix, m_viewMatrix, m_projectionMatrix, m_texture;

	BasicShader() : ShaderBase("TestShader", Common::FolderManager::GetInstance()->GetShadersPath() + "testVS.glsl",
		Common::FolderManager::GetInstance()->GetShadersPath() + "testFS.glsl")
	{
		Init();
	}

	virtual void AccessAllUniformLocations()  override
	{
		m_worldMatrix = GetUniform("worldMatrix");
		m_viewMatrix = GetUniform("viewMatrix");
		m_projectionMatrix = GetUniform("projectionMatrix");
		m_texture = GetUniform("albedo");
	}

	void SetTransformationMatrices(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		m_worldMatrix.LoadUniform(worldMatrix);
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
	Resources::PoolBase<Graphics::Mesh::Skin, std::string, Resources::MeshAllocationPolicy> m_meshPool;
	Resources::PoolBase<Graphics::Texture::ITexture, std::string, Resources::TextureAllocationPolicy> m_texturePool;

	BasicShader m_shader;
	std::shared_ptr<Graphics::Texture::ITexture> m_texture;
	std::shared_ptr<Graphics::Mesh::Skin> m_skin;

	glm::mat4 m_viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 worldMatrix;
	float rotateAngle = 0.0f;

	std::vector<Actor> m_allActors;

public:
	Engine();
	~Engine();

	void TickWindow();

	void UpdateWorldMatrix();
};

