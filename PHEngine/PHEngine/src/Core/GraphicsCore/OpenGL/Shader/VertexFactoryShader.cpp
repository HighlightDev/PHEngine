#include "VertexFactoryShader.h"

#include "Core/UtilityCore/StringStreamWrapper.h"

namespace Graphics
{
   namespace OpenGL
   {

      VertexFactoryShader::VertexFactoryShader(const std::string& pathToVertexFactoryShader, const std::string& vertexFactoryName)
         : IShader(vertexFactoryName)
      {
         LoadShaderSource(pathToVertexFactoryShader);
      }

      VertexFactoryShader::~VertexFactoryShader()
      {
      }


      void VertexFactoryShader::ProcessAllPredefines()
      {

      }

      void VertexFactoryShader::AccessAllUniformLocations(uint32_t shaderProgramID)
      {

      }

      void VertexFactoryShader::SetShaderPredefine()
      {

      }

      void VertexFactoryShader::LoadShaderSource(const std::string& pathToVertexFactoryShader)
      {
         auto shaderSrcVec = Base::LoadShaderSrc(pathToVertexFactoryShader);

         EngineUtility::StringStreamWrapper::FlushString();
         for (auto srcItem : shaderSrcVec)
         {
            EngineUtility::StringStreamWrapper::ToString(srcItem);
         }

         mShaderSource = EngineUtility::StringStreamWrapper::FlushString();

      }

   }
}