#pragma once
#include "IMaterial.h"

namespace Graphics
{

   class DynamicMaterial :
      public IMaterial
   {
   public:
      DynamicMaterial(const std::string& materialName, const std::string& relativeMaterialShaderPath);
      ~DynamicMaterial();
   };

}

