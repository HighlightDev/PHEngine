#pragma once
#include "IShader.h"

namespace Graphics
{
   namespace OpenGL
   {

      class VertexFactoryShader :
         public IShader
      {
         using Base = IShader;

         std::string mShaderSource;

      public:

         VertexFactoryShader(const std::string& pathToVertexFactoryShader, const std::string& vertexFactoryName);

         virtual ~VertexFactoryShader();

      protected:

         virtual void ProcessAllPredefines() override;
         virtual void AccessAllUniformLocations(uint32_t shaderProgramID) override;
         virtual void SetShaderPredefine() override;

      private:
         void LoadShaderSource(const std::string& pathToVertexFactoryShader);
      };

   }
}

