#include "PointLight.h"

namespace Graphics
{
   namespace Light
   {

      PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
         glm::vec3 attenuation)
         : LightBase(ambient, diffuse, specular)
         , Position(position)
         , Attenuation(attenuation)
         , RadianceRadius(1000.0f)
      {
      }

      PointLight::~PointLight()
      {
      }

   }
}
