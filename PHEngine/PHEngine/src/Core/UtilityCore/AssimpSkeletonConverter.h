#pragma once

#include <assimp/matrix4x4.h>
#include <glm/mat4x4.hpp>

#include "Core/GraphicsCore/Animation/Bone.h"
#include "Core/IoCore/MeshLoaderCore/AssimpLoader/SkeletonBoneLOADER.h"

using namespace Graphics::Animation;
using namespace Io::MeshLoader::Assimp;

namespace EngineUtility
{

	class AssimpSkeletonConverter
	{

		static std::unique_ptr<AssimpSkeletonConverter> m_instance;

	public:

		AssimpSkeletonConverter();

		~AssimpSkeletonConverter();

		static std::unique_ptr<AssimpSkeletonConverter>& GetInstance()
		{
			if (!m_instance)
				m_instance = std::make_unique<AssimpSkeletonConverter>();

			return m_instance;
		}

		Bone* ConvertAssimpBoneToEngineBone(SkeletonBoneBaseLOADER* rootBone);

	private:

		glm::mat4&& ConvertAssimpMatrix4x4ToOpenTKMatrix4(aiMatrix4x4& srcMatrix);

		void IterateBoneTree(Bone& dstParentBone, SkeletonBoneLOADER* srcParentNode);
	};

}

