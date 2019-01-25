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
#include "Core/GameCore/FirstPersonCamera.h"

using namespace Game;
using namespace Graphics::OpenGL;

class Engine
{
	FirstPersonCamera camera;

	glm::mat4 m_viewMatrix;
	glm::mat4 projectionMatrix;

	std::vector<Actor> m_allActors;

public:
	Engine();
	~Engine();

	void TickWindow();

	void MouseMove();
};

