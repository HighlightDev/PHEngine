#pragma once

#include "Core/GraphicsCore/Texture/ITexture.h"

#include <string>
#include <set>
#include <tuple>
#include <unordered_map>

using namespace Graphics::Texture;

namespace Graphics
{
   struct IMaterialParams
   {
   };

   class IMaterial
   {
   protected:

      std::unordered_map<std::string, MaterialProperty> mProperties;

   public:

      IMaterial();

      virtual ~IMaterial();
   };

}

