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

         auto src = LoadShaderSrc(absoluteShaderPath);
         for (auto item : src)
         {
            EngineUtility::StringStreamWrapper::ToString(item, '\n');
         }
         std::string shaderSource = std::move(EngineUtility::StringStreamWrapper::FlushString());

         mShaderSource = shaderSource;
      }

      void IMaterialShader::ProcessAllPredefines()
      {
         // This is an implementation in base class
      }

      void IMaterialShader::SetShaderPredefine()
      {
         // This is an implementation in base class
      }

      void IMaterialShader::AccessAllUniformLocations(uint32_t shaderProgramID)
      {
         // This is an implementation in base class
      }
   }
}
