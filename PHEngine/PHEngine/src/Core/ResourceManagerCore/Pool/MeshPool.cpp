#include "MeshPool.h"


namespace Resources
{

	std::unique_ptr<MeshPool> MeshPool::m_instance;

	MeshPool::MeshPool()
		: PoolBase<Skin, std::string, MeshAllocationPolicy>()
	{
	}


	MeshPool::~MeshPool()
	{
	}

}
