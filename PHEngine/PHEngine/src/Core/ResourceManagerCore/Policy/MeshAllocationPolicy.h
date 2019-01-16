#pragma once
#include <string>
#include <memory>

#include "Core/GraphicsCore/OpenGL/VertexArrayObject.h"

using namespace Graphics::OpenGL;

namespace Resources
{

	class MeshAllocationPolicy
	{
	public:

		MeshAllocationPolicy();
		~MeshAllocationPolicy();

		static std::shared_ptr<VertexArrayObject> AllocateMemory(std::string& arg);

		static void DeallocateMemory(std::shared_ptr<VertexArrayObject> arg);
	};

}

