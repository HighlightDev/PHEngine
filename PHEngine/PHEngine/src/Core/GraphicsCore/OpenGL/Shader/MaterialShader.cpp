#include "MaterialShader.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/UtilityCore/StringStreamWrapper.h"

#include <fstream>

namespace Graphics
{
   namespace OpenGL
   {

      IMaterialShader::IMaterialShader(const std::string& materialName)
         : IShader(materialName)
      {
      }


      IMaterialShader::~IMaterialShader()
      {
      }

      void IMaterialShader::InitMaterialShader(const IMaterial* material)
      {
         LoadMaterialShaderSource(material);
         ProcessAllPredefines();
      }

      void IMaterialShader::LoadMaterialShaderSource(const IMaterial* material)
      {
         std::string absoluteShaderPath = EngineUtility::ConvertFromRelativeToAbsolutePath(material->GetShaderPath());
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
   }
}
