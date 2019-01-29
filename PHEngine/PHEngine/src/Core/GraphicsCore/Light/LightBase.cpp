#include "LightBase.h"

namespace Graphics
{
   namespace Light
   {

      LightBase::LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
         : Ambient(ambient)
         , Diffuse(diffuse)
         , Specular(specular)
      {
      }

      LightBase::~LightBase()
      {
      }

   }
}