#include "MaterialShader.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/UtilityCore/StringStreamWrapper.h"

#include <fstream>

namespace Graphics
{
   namespace OpenGL
   {

      IMaterialShader::IMaterialShader()
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

         std::ifstream stream(absoluteShaderPath);
         std::string line = std::string("");

         while (stream.is_open() && getline(stream, line))
         {
            EngineUtility::StringStreamWrapper::ToString(line, '\n');
         }

         std::string shaderSource = std::move(EngineUtility::StringStreamWrapper::FlushString());

         mShaderSource = shaderSource;
      }

      void IMaterialShader::ProcessAllPredefines()
      {

      }
   }
}
