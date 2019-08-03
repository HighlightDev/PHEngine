#pragma once

#include "Core/GraphicsCore/Material/IMaterial.h"
#include "Core/GraphicsCore/OpenGL/Shader/IShader.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderPredefineUtility.h"

namespace Graphics
{
   namespace OpenGL
   {
      class IMaterialShader 
         : public IShader
      {
         std::string mShaderSource;

         std::vector<ShaderGenericDefineConstant> mConstantDefines;
         std::vector<ShaderGenericDefine> mDefines;

      public:

         IMaterialShader(const std::string& materialName);

         virtual ~IMaterialShader();

         std::string GetShaderSource() const;

         template <typename ValueType>
         void DefineConstant(const std::string& name, ValueType&& value)
         {
            std::string formatedValue = MacroConverter<ValueType>::GetValue(std::forward<ValueType>(value));
            mConstantDefines.emplace_back(ShaderGenericDefineConstant(name, formatedValue));
         }

         void Define(const std::string& name);

         void Undefine(const std::string& name);

      protected:

         void InitMaterialShader(const std::string& pathToMaterialShader);

         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) override;

      private:

         void LoadMaterialShaderSource(const std::string& relativePathToMaterialShader);
      };

      /* META DATA */
      template <size_t indexCount>
      struct SetUniformValuesIterate
      {

         template <typename ValuesTuple, typename UniformArray>
         static void ProcessSet(ValuesTuple values, UniformArray uniforms)
         {
            uniforms[indexCount].LoadUniform(std::get<indexCount>(values).GetValue(indexCount));
            SetUniformValuesIterate<indexCount - 1>::ProcessSet(values, uniforms);
         }
      };

      template <>
      struct SetUniformValuesIterate<0>
      {
         template <typename ValuesTuple, typename UniformArray>
         static void ProcessSet(ValuesTuple values, UniformArray uniforms)
         {
            uniforms[0].LoadUniform(std::get<0>(values).GetValue(0));
         }
      };
      /* META DATA */

      template <typename MaterialT>
      class MaterialShaderImp
         : public IMaterialShader
      {
      public :

         using materialInstance_t = MaterialT;

         static constexpr size_t properties_count = std::tuple_size<typename materialInstance_t::materialProperties_t>::value;

         using uniforms_t = std::array<Uniform, properties_count>;

         uniforms_t Uniforms;

         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) override;

         void SetUniformValues(const MaterialT& materialInstance);

         MaterialShaderImp();

      private:

         std::array<std::string, properties_count> mPropertyNames;
      };
   }
}

