#pragma once

#include "Core/GraphicsCore/Texture/ITexture.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

using namespace Graphics::Texture;
using namespace Graphics::OpenGL;

struct MaterialProperty
{
public:

   virtual void SetValueToUniform(Uniform uniform, const int32_t propertyIndex) const = 0;

};

struct TextureMaterialProperty 
   : public MaterialProperty
{
   using MaterialPropertyValueType = std::shared_ptr<ITexture>;

private :

   MaterialPropertyValueType m_value;

public:

   TextureMaterialProperty(MaterialPropertyValueType propertyValue)
      : MaterialProperty()
      , m_value(propertyValue)
   {
   }

   virtual void SetValueToUniform(Uniform uniform, const int32_t propertyIndex) const override
   {
      int32_t slot = 10 + propertyIndex;
      m_value->BindTexture(slot);
      uniform.LoadUniform(slot);
   }
};
