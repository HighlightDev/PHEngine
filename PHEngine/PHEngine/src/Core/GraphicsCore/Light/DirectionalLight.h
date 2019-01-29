#pragma once
#include "LightBase.h"

namespace Graphics
{
   namespace Light
   {

      class DirectionalLight :
         public LightBase
      {
      public:

         glm::vec3 Direction;

         DirectionalLight(glm::vec3 lightDir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

         ~DirectionalLight();
      };

   }
}

