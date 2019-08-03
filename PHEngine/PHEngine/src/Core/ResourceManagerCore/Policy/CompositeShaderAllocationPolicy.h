#pragma once

#include <memory>
#include <vector>

#include "Core/GraphicsCore/OpenGL/Shader/IShader.h"
#include "Core/GraphicsCore/OpenGL/Shader/CompositeShaderParams.h"

using namespace Graphics::OpenGL;

namespace Resources
{

   template <typename ModelT>
   struct CompositeShaderAllocationPolicy
   {
      template <typename CompositeShaderType>
      static std::shared_ptr<IShader> AllocateMemory(const CompositeShaderParams& arg)
      {
         return std::make_shared<CompositeShaderType>(arg.ShaderName, arg.mShader);
      }

      static void DeallocateMemory(std::shared_ptr<IShader> arg)
      {
         arg->CleanUp(true);
      }
   };

}