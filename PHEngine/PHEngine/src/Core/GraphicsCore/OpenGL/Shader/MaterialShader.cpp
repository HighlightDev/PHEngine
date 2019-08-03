#include "MaterialShader.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/GraphicsCore/Material/PBRMaterial.h"

#include <fstream>
#include <type_traits>

namespace Graphics
{
   namespace OpenGL
   {
      /************************************************************************/
      /*                                 IMaterialShader                      */
      /************************************************************************/
      IMaterialShader::IMaterialShader(const std::string& materialName)
         : IShader(materialName)
      {
      }

      IMaterialShader::~IMaterialShader()
      {
      }

      void IMaterialShader::InitMaterialShader(const std::string& relativePathToMaterialShader)
      {
         LoadMaterialShaderSource(relativePathToMaterialShader);
      }

      void IMaterialShader::LoadMaterialShaderSource(const std::string& relativePathToMaterialShader)
      {
         std::string absoluteShaderPath = EngineUtility::ConvertFromRelativeToAbsolutePath(relativePathToMaterialShader);
         mShaderSource = LoadShaderSource(absoluteShaderPath);
      }

      std::string IMaterialShader::GetShaderSource() const
      {
         return mShaderSource;
      }

      void IMaterialShader::AccessAllUniformLocations(uint32_t shaderProgramID)
      {
         // This is an implementation in base class
      }

      void IMaterialShader::Define(const std::string& name)
      {
         mDefines.emplace_back(name, true);
      }

      void IMaterialShader::Undefine(const std::string& name)
      {
         mDefines.emplace_back(name, false);
      }

      /************************************************************************/
      /*                               MaterialShaderImpl                     */
      /************************************************************************/

      template class MaterialShaderImp<PBRMaterial>;

      template <typename MaterialT>
      void MaterialShaderImp<MaterialT>::AccessAllUniformLocations(uint32_t shaderProgramID)
      {
         for (size_t index = 0; index < properties_count; ++index)
         {
            Uniforms[index] = GetUniform(mPropertyNames[index], shaderProgramID);
         }
      }

      template <typename MaterialT>
      void MaterialShaderImp<MaterialT>::SetUniformValues(const MaterialT& materialInstance)
      {
         SetUniformValuesIterate<properties_count - 1>::ProcessSet(materialInstance.mProperties, Uniforms);
      }

      template <typename MaterialT>
      MaterialShaderImp<MaterialT>::MaterialShaderImp()
         : IMaterialShader(MaterialT::mMaterialName)
         , mPropertyNames(MaterialT::mPropertiesName)
      {
#ifdef DEBUG
         static_assert(std::is_base_of<IMaterial, MaterialT>::value, "Material type must be child if IMaterial!");
#endif

         InitMaterialShader(MaterialT::mRelativeMaterialShaderPath);
      }
   }
}
