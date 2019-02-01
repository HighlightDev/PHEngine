#include "ScreenQuad.h"
#include "Core/GraphicsCore/OpenGL/VertexBufferObject.h"
#include "Core/GraphicsCore/OpenGL/VertexBufferObjectBase.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"
#include "Core/CommonCore/FolderManager.h"

#include <memory>
#include <vector>
#include <utility>

using namespace Common;
using namespace Resources;

namespace Graphics
{

	ScreenQuad* ScreenQuad::m_instance = nullptr;

	ScreenQuad::ScreenQuad()
	{
		Init();
	}

	ScreenQuad::~ScreenQuad()
	{
      ShaderPool::GetInstance()->TryToFreeMemory(m_resolveTexShader);
	}

	void ScreenQuad::Init()
	{
      std::string resolveTexShaderPath = Common::FolderManager::GetInstance()->GetShadersPath() + "resolveTextureVS.glsl" + "," + Common::FolderManager::GetInstance()->GetShadersPath() + "resolveTextureFS.glsl";
      m_resolveTexShader = std::dynamic_pointer_cast<ResolveTextureShader>(ShaderPool::GetInstance()->GetOrAllocateResource<ResolveTextureShader>(resolveTexShaderPath));

		/*Screen fill quad*/
		std::vector<float> vertices = { -1.0f, -1.0f, 0.0f ,
			 1.0f, -1.0f, 0.0f ,
			 1.0f, 1.0f, 0.0f ,
			 1.0f, 1.0f, 0.0f ,
			-1.0f, 1.0f, 0.0f ,
			-1.0f, -1.0f, 0.0f };

		std::vector<float> texCoords = { 0, 1 ,
			 1, 1 ,
			 1, 0 ,
			 1, 0 ,
			 0, 0 ,
			 0, 1 };

		VertexBufferObjectBase* verticesVBO = new VertexBufferObject< float, 3, GL_FLOAT>(std::make_shared<std::vector<float>>(vertices), GL_ARRAY_BUFFER, 0, DataCarryFlag::Invalidate);
		VertexBufferObjectBase* texCoordsVBO = new VertexBufferObject< float, 2, GL_FLOAT>(std::make_shared<std::vector<float>>(texCoords), GL_ARRAY_BUFFER, 1, DataCarryFlag::Invalidate);

		m_vao.AddVBO(std::unique_ptr<VertexBufferObjectBase>(std::move(verticesVBO)),
			std::unique_ptr<VertexBufferObjectBase>(std::move(texCoordsVBO)));

		m_vao.BindBuffersToVao();
	}
}
