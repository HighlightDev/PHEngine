#pragma once

#include "LightRenderData.h"

namespace Graphics
{
   namespace Data
   {
      struct PointLightRenderData 
         : public LightRenderData
      {

         glm::vec3 Position;
         glm::vec3 Attenuation;

         float RadianceSqrRadius;

         PointLightRenderData(glm::vec3 position, glm::vec3 attenuation, float radianceSqrRadius,
            glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : LightRenderData(ambient, diffuse, specular)
            , Position(position)
            , Attenuation(attenuation)
            , RadianceSqrRadius(radianceSqrRadius)
         {
         }

         ~PointLightRenderData()
         {
         }
      };
   }
}
