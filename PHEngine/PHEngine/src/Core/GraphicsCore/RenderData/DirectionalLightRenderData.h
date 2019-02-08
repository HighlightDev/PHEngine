#pragma once

#include <memory>

#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

using namespace Graphics::Mesh;
using namespace Graphics::OpenGL;
using namespace Graphics::Texture;

namespace Graphics
{
   namespace Data
   {

      struct DirectionalLightRenderData
      {

         glm::vec3 Direction;
         glm::vec3 Ambient;
         glm::vec3 Diffuse;
         glm::vec3 Specular;

         DirectionalLightRenderData(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : Direction(direction)
            , Ambient(ambient)
            , Diffuse(diffuse)
            , Specular(specular)
         {
         }
         ~DirectionalLightRenderData()
         {
         }
      };
   }
}
