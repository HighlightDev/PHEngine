#pragma once

#include "Core/GraphicsCore/Texture/ITexture.h"

#include <string>
#include <set>
#include <tuple>

using namespace Graphics::Texture;

namespace Graphics
{
   enum class PropertyType
   {
      TEXTURE,
      INT,
      UINT,
      FLOAT
   };

   struct TexturePropertyProcessPolicy
   {
      using property_t = std::shared_ptr<ITexture>;
      using propertyValue_t = int32_t;

      static propertyValue_t Process(property_t property, size_t propertyIndex)
      {
         property->BindTexture(10 + propertyIndex); // 10 is in case previous texture slots are not available
         return propertyIndex + 10;
      }
   };

   template <typename PropertyProcessPolicy>
   struct MaterialProperty;

   template <typename PropertyProcessPolicy>
   struct MaterialProperty
   {
   public:

      using propertyValue_t = typename PropertyProcessPolicy::propertyValue_t;
      using property_t = typename PropertyProcessPolicy::property_t;

   private:

      property_t mProperty;

   public:

      template <typename PropertyType>
      MaterialProperty(PropertyType&& propValue)
      {
         mProperty = std::forward<PropertyType>(propValue);
      }

      propertyValue_t GetValue(size_t propertyIndex)
      {
         return PropertyProcessPolicy::Process(mProperty, propertyIndex);
      }

   };

   class IMaterial
   {
   protected:

      std::string mMaterialShaderPath;

   public:

      IMaterial();

      virtual ~IMaterial();

      std::string GetShaderPath() const;
   };

}

