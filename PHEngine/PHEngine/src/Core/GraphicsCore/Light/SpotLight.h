#pragma once
#include "LightBase.h"
#include <glm/vec3.hpp>

namespace Graphics
{
   namespace Light
   {

      class SpotLight :
         public LightBase
      {
      public:

         glm::vec3 Position;
         glm::vec3 Direction;
         glm::vec3 Attenuation;
         float SpotCutoff;
         float SpotExponent;

         SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
            glm::vec3 specular, glm::vec3 attenuation, float spotCutoff, float spotExponent);

         virtual ~SpotLight();
      };

   }
}

