#include "IMaterial.h"

namespace Graphics
{

   IMaterial::IMaterial()
   {
   }

   IMaterial::~IMaterial()
   {
   }

   std::string IMaterial::GetShaderPath() const
   {
      return mMaterialShaderPath;
   }
}
