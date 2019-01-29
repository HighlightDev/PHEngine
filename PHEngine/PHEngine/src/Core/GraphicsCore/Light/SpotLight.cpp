#include "SpotLight.h"

namespace Graphics
{
   namespace Light
   {

      SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
         glm::vec3 specular, glm::vec3 attenuation, float spotCutoff, float spotExponent)
         : LightBase(ambient, diffuse, specular)
         , Position(position)
         , Direction(direction)
         , Attenuation(attenuation)
         , SpotCutoff(spotCutoff)
         , SpotExponent(spotExponent)
      {
      }

      SpotLight::~SpotLight()
      {
      }

   }
}