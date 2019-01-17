#pragma once

#include <string>
#include <vector>

#include "AnimationFrameLOADER.h"

namespace MeshLoader
{
	namespace Assimp
	{

		struct BoneFrameCollectionLOADER
		{
			std::string BoneName;
			std::vector<AnimationFrameLOADER> Frames;

		public:
			BoneFrameCollectionLOADER(std::string&& name);

			~BoneFrameCollectionLOADER();

			void AddFrame(AnimationFrameLOADER&& frame);
		};

	}
}
