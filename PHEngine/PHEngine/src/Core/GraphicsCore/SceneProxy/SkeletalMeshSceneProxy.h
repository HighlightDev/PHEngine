#pragma once
#include "PrimitiveSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      class SkeletalMeshSceneProxy :
         public PrimitiveSceneProxy
      {
      public:
         SkeletalMeshSceneProxy();
         virtual ~SkeletalMeshSceneProxy();
      };

   }
}
