#include "AnimationAllocationPolicy.h"

#include "Core/IoCore/MeshLoaderCore/AssimpLoader/AssimpMeshLoader.h"
#include "Core/UtilityCore/AssimpSkeletonConverter.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"

using namespace Io::MeshLoader::Assimp;
using namespace EngineUtility;

namespace Resources
{

   std::shared_ptr<std::vector<AnimationSequence>> AnimationAllocationPolicy::AllocateMemory(std::string& arg)
   {
      const int32_t countOfBonesInfluencingOnVertex = 3;

      std::vector<AnimationSequence> resultAnimationCollection;

      std::string absolutePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(arg));
      AssimpMeshLoader<countOfBonesInfluencingOnVertex> loader(absolutePath);
      if (loader.GetHasAnimationData())
      {
         MeshAnimationData& animationData = loader.GetAnimationData();
         resultAnimationCollection = EngineUtility::AssimpSkeletonConverter::GetInstance()->ConvertAssimpAnimationToEngineAnimation(animationData.GetAnimations());
      }

      return std::make_shared<std::vector<AnimationSequence>>(std::move(resultAnimationCollection));
   }

   void AnimationAllocationPolicy::DeallocateMemory(std::shared_ptr<std::vector<AnimationSequence>> arg)
   {
   }

}