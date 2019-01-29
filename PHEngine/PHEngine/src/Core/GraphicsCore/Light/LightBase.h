#pragma once

#include <glm/vec3.hpp>

namespace Graphics
{
   namespace Light
   {

      class LightBase
      {
      public:

         glm::vec3 Ambient;
         glm::vec3 Diffuse;
         glm::vec3 Specular;

         LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

         virtual ~LightBase();
      };

   }
}

