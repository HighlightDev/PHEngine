#include "PBRMaterial.h"

namespace Graphics
{

   PBRMaterial::PBRMaterial(ITextureShared albedo,
      ITextureShared normalMap,
      ITextureShared metallicMap,
      ITextureShared roughnessMap,
      ITextureShared ambientOcclusionMap)
      : IMaterial()
      , mProperties(std::make_tuple(albedo, normalMap, metallicMap, roughnessMap, ambientOcclusionMap))
   {
   }

   PBRMaterial::~PBRMaterial()
   {

   }

}
