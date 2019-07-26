#pragma once
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/OpenGL/Shader/Uniform.h"

#include <string>

using namespace Graphics::OpenGL;

namespace Game
{
   namespace ShaderImpl
   {
      class CompositeDeferredCollectShader :
         public ShaderBase
      {

         using Base = ShaderBase;

      public:

         CompositeDeferredCollectShader(const ShaderParams& params)
            : ShaderBase(params)
         {
         }

         void AccessAllUniformLocations(uint32_t shaderProgramId)
         {
            Base::AccessAllUniformLocations(shaderProgramId);
         }
      };

   }
}

