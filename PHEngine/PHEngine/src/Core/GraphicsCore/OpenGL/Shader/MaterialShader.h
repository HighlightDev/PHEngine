#pragma once

#include "Core/GraphicsCore/Material/IMaterial.h"
#include "Core/GraphicsCore/OpenGL/Shader/IShader.h"

namespace Graphics
{
   namespace OpenGL
   {
      class IMaterialShader 
         : public IShader
      {
         std::string mShaderSource;

      public:

         IMaterialShader(const std::string& materialName);

         virtual ~IMaterialShader();

      protected:

         void InitMaterialShader(const IMaterial* material);

         void LoadMaterialShaderSource(const IMaterial* material);

         virtual void ProcessAllPredefines() override;
         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) override;
         virtual void SetShaderPredefine() override;
      };

      template <typename ValuesTuple, typename UniformArray, size_t indexCount>
      struct SetUniformValuesIterate
      {

         static void ProcessSet(ValuesTuple& values, UniformArray& uniforms)
         {
            uniforms[indexCount].LoadUniform(std::get<indexCount>(values).GetValue(indexCount));
            SetUniformValuesIterate<ValuesTuple, UniformArray, indexCount - 1>::ProcessSet(values, uniforms);
         }
      };

      template <typename ValuesTuple, typename UniformArray>
      struct SetUniformValuesIterate<ValuesTuple, UniformArray, 0>
      {
         static void ProcessSet(ValuesTuple& values, UniformArray& uniforms)
         {
            uniforms[0].LoadUniform(std::get<0>(values).GetValue(0));
         }
      };

      template <typename Material>
      class MaterialShaderImp
         : public IMaterialShader
      {
      public :

         using material_t = Material;

         static constexpr size_t properties_count = std::tuple_size<typename material_t::materialProperties_t>::value;

         using uniforms_t = std::array<Uniform, properties_count>;

         material_t mMaterial;
         uniforms_t Uniforms;

         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) override
         {
            for (size_t index = 0; index < properties_count; ++index)
            {
               Uniforms[index] = GetUniform(mMaterial.mPropertiesName[index], shaderProgramID);
            }
         }

         void SetUniformValues()
         {
            SetUniformValuesIterate<typename material_t::materialProperties_t, uniforms_t, properties_count - 1>::ProcessSet(mMaterial.mProperties, Uniforms);
         }

         MaterialShaderImp(const Material& material)
            : IMaterialShader(material.mMaterialName)
            , mMaterial(material)
         {
         }
      };
   }
}

