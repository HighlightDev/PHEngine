#pragma once

#include "IMaterial.h"
#include "MaterialProperty.h"
#include "MaterialProcessPolicy.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

#include <memory>
#include <array>

using namespace Graphics::Texture;

namespace Graphics
{

   struct PBRMaterialParams 
      : public IMaterialParams
   {
      using ITextureShared = std::shared_ptr<ITexture>;

      ITextureShared mAlbedo;
      ITextureShared mNormalMap;
      ITextureShared mMetallicMap;
      ITextureShared mRoughnessMap;
      ITextureShared mAmbientOcclusionMap;

      PBRMaterialParams(
         ITextureShared albedo,
         ITextureShared normalMap,
         ITextureShared metallicMap,
         ITextureShared roughnessMap,
         ITextureShared ambientOcclusionMap)
         : mAlbedo(albedo)
         , mNormalMap(normalMap)
         , mMetallicMap(metallicMap)
         , mRoughnessMap(roughnessMap)
         , mAmbientOcclusionMap(ambientOcclusionMap)
      {
      }
   };

   class PBRMaterial :
      public IMaterial
   {
   public:

      using ITextureShared = std::shared_ptr<ITexture>;

      using materialProperties_t = std::tuple<MaterialProperty<TexturePropertyProcessPolicy>,
         MaterialProperty<TexturePropertyProcessPolicy>,
         MaterialProperty<TexturePropertyProcessPolicy>,
         MaterialProperty<TexturePropertyProcessPolicy>,
         MaterialProperty<TexturePropertyProcessPolicy>>;

      materialProperties_t mProperties;

      PBRMaterial(std::shared_ptr<IMaterialParams> materialParams);

      PBRMaterial(const PBRMaterial&) = default;
      PBRMaterial& operator=(const PBRMaterial&) = default;

      virtual ~PBRMaterial();

   public:

      static const std::string mMaterialName;
      static const std::array<std::string, std::tuple_size<materialProperties_t>::value> mPropertiesName;
      static const std::string mRelativeMaterialShaderPath;
   };

}

