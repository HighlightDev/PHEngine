#pragma once

using namespace Graphics::Texture;

enum class PropertyType
{
   TEXTURE,
   INT,
   UINT,
   FLOAT
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
