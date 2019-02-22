#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Graphics
{

   struct ProjectedShadowInfo
   {
      glm::mat4x4 ProjectionViewMatrix;

      glm::vec3 offset;

      ProjectedShadowInfo();

      ~ProjectedShadowInfo();
   };

}

