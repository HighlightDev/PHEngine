#pragma once
#include "LightBase.h"

#include <glm/vec3.hpp>

namespace Graphics
{
   namespace Light
   {

      class PointLight :
         public LightBase
      {
      public:

         glm::vec3 Position;
         glm::vec3 Attenuation;

         float RadianceRadius;

         PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
            glm::vec3 attenuation);

         ~PointLight();
      };

   }
}

