#include "MeshAllocationPolicy.h"
namespace Resources
{

	MeshAllocationPolicy::MeshAllocationPolicy()
	{
	}


	MeshAllocationPolicy::~MeshAllocationPolicy()
	{
	}

	MeshAllocationPolicy::sharedValue_t MeshAllocationPolicy::AllocateMemory(std::string arg)
	{
		return std::make_unique<int>(5);
	}

	void MeshAllocationPolicy::DeallocateMemory(sharedValue_t arg)
	{

	}

}
