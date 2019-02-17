#pragma once

#include <glm/vec3.hpp>

namespace Graphics
{
   namespace Data
   {

      struct LightRenderData
      {

         glm::vec3 Ambient;
         glm::vec3 Diffuse;
         glm::vec3 Specular;

         LightRenderData(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : Ambient(ambient)
            , Diffuse(diffuse)
            , Specular(specular)
         {
         }

         ~LightRenderData()
         {
         }
      };
   }
}
