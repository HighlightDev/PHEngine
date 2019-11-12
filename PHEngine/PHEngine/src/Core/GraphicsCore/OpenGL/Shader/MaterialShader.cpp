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
      IMaterialShader::IMaterialShader(std::shared_ptr<IMaterial> materialInstance)
         : IShader(materialInstance->MaterialName)
         , mMaterialInstance(materialInstance)
      {
         InitMaterialShader(materialInstance->RelativeMaterialShaderPath);
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

      void MaterialShaderImp::AccessAllUniformLocations(uint32_t shaderProgramID)
      {
         for (auto namePlusPropPair : mMaterialInstance->GetProperties())
         {
            UniformsMap[namePlusPropPair.first] = GetUniform(namePlusPropPair.first, shaderProgramID);
         }
      }

      void MaterialShaderImp::SetUniformValues()
      {
         int32_t index = 0;
         for (auto namePlusPropPair : mMaterialInstance->GetProperties())
         {
            namePlusPropPair.second->SetValueToUniform(UniformsMap[namePlusPropPair.first], index);
            ++index;
         }
      }

      MaterialShaderImp::MaterialShaderImp(std::shared_ptr<IMaterial> materialInstance)
         : IMaterialShader(materialInstance)
      {
      }
   }
}
