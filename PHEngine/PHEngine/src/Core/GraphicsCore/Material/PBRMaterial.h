#pragma once

#include "IMaterial.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

#include <memory>
#include <array>

using namespace Graphics::Texture;

namespace Graphics
{

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
      const std::string mMaterialName = "PBR Material";

      const std::array<std::string, std::tuple_size<materialProperties_t>::value> mPropertiesName = { "albedo", "normalMap", "metallicMap", "roughnessMap", "ambientOcclusionMap" };

      PBRMaterial(ITextureShared albedo,
                  ITextureShared normalMap,
                  ITextureShared metallicMap,
                  ITextureShared roughnessMap,
                  ITextureShared ambientOcclusionMap);

      PBRMaterial(const PBRMaterial&) = default;
      PBRMaterial& operator=(const PBRMaterial&) = default;

      virtual ~PBRMaterial();
   };

}

