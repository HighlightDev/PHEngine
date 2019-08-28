#include "PBRMaterial.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/CommonCore/FolderManager.h"

namespace Graphics
{
   const std::string PBRMaterial::mMaterialName = "PBR Material";
   const std::string PBRMaterial::mRelativeMaterialShaderPath = Common::FolderManager::GetInstance()->GetShadersPath() + "\\material_shaders\\PBRMaterial.glsl";
   const std::array<std::string, std::tuple_size<typename PBRMaterial::materialProperties_t>::value> PBRMaterial::mPropertiesName = { "albedo", "normalMap", "metallicMap", "roughnessMap", "ambientOcclusionMap" };

   PBRMaterial::PBRMaterial(std::shared_ptr<IMaterialParams> materialParams)
      : IMaterial()
      , mProperties(std::make_tuple(
         std::static_pointer_cast<PBRMaterialParams>(materialParams)->mAlbedo,
         std::static_pointer_cast<PBRMaterialParams>(materialParams)->mNormalMap, 
         std::static_pointer_cast<PBRMaterialParams>(materialParams)->mMetallicMap, 
         std::static_pointer_cast<PBRMaterialParams>(materialParams)->mRoughnessMap, 
         std::static_pointer_cast<PBRMaterialParams>(materialParams)->mAmbientOcclusionMap))
   {
   }

   PBRMaterial::~PBRMaterial()
   {
   }

}
