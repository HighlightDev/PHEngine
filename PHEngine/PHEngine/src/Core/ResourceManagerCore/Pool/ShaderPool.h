#pragma once
#include "PoolBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/ResourceManagerCore/Policy/ShaderAllocationPolicy.h"

#include <string>

using namespace Graphics::OpenGL;

namespace Resources
{

	class ShaderPool :
		public PoolBase<ShaderBase, std::string, ShaderAllocationPolicy>
	{
		static std::unique_ptr<ShaderPool> m_instance;

	public:

		using poolType_t = PoolBase<ShaderBase, std::string, ShaderAllocationPolicy>;

		static std::unique_ptr<ShaderPool>& GetInstance()
		{
			if (!m_instance)
				m_instance = std::make_unique<ShaderPool>();

			return m_instance;
		}

		static void ReloadInstance()
		{
			if (m_instance)
				m_instance.reset();
		}

		ShaderPool();

		virtual ~ShaderPool();
	};
}
