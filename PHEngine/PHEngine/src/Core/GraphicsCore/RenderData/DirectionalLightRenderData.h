#pragma once

#include "LightRenderData.h"

namespace Graphics
{
   namespace Data
   {

      struct DirectionalLightRenderData 
         : public LightRenderData
      {

         glm::vec3 Direction;

         DirectionalLightRenderData(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : LightRenderData(ambient, diffuse, specular)
            , Direction(direction)
         {
         }
         ~DirectionalLightRenderData()
         {
         }
      };
   }
}
