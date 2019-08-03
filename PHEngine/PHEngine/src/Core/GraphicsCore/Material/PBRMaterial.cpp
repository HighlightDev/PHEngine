#include "PBRMaterial.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/CommonCore/FolderManager.h"

namespace Graphics
{
   const std::string PBRMaterial::mMaterialName = "PBR Material";
   const std::string PBRMaterial::mRelativeMaterialShaderPath = Common::FolderManager::GetInstance()->GetShadersPath() + "\\material_shaders\\PBRMaterial.glsl";
   const std::array<std::string, std::tuple_size<typename PBRMaterial::materialProperties_t>::value> PBRMaterial::mPropertiesName = { "albedo", "normalMap", "metallicMap", "roughnessMap", "ambientOcclusionMap" };

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
