#include "DirectionalLight.h"

namespace Graphics
{
   namespace Light
   {

      DirectionalLight::DirectionalLight(glm::vec3 lightDir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
         : LightBase(ambient, diffuse,specular)
         , Direction(lightDir)
      {
      }

      DirectionalLight::~DirectionalLight()
      {
      }

   }
}