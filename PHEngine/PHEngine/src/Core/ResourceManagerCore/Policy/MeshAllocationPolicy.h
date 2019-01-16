#pragma once
#include <string>
#include <memory>

namespace Resources
{

	class MeshAllocationPolicy
	{
	public:

		using sharedValue_t = std::shared_ptr<int>;

		MeshAllocationPolicy();
		~MeshAllocationPolicy();

		static sharedValue_t AllocateMemory(std::string arg);

		static void DeallocateMemory(sharedValue_t arg);
	};

}

