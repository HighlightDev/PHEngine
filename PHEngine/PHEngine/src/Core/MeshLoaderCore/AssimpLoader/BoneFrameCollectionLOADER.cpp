#include "BoneFrameCollectionLOADER.h"

namespace MeshLoader
{
	namespace Assimp
	{

		BoneFrameCollectionLOADER::BoneFrameCollectionLOADER(std::string&& name)
			: BoneName(std::move(name))
		{
			
		}

		BoneFrameCollectionLOADER::~BoneFrameCollectionLOADER()
		{

		}

		void BoneFrameCollectionLOADER::AddFrame(AnimationFrameLOADER&& frame)
		{
			Frames.emplace_back(std::move(frame));
		}
	}
}