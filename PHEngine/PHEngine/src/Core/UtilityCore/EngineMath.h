#pragma once

#include <algorithm>

namespace EngineUtility
{
#define AXIS_RIGHT     glm::vec3(1, 0, 0)
#define AXIS_UP	       glm::vec3(0, 1, 0)
#define AXIS_FORWARD   glm::vec3(0, 0, 1)

   constexpr auto PI = 3.14f;
   /************************************************************************/
   /*                       Convert from degrees to radians                */
   /************************************************************************/

#define DEG_TO_RAD(X) (X * (PI / 180.0f))

   /************************************************************************/
   /*                       Convert from radians to degrees                */
   /************************************************************************/
#define RAD_TO_DEG(X) (X * (180.0f / PI))

   constexpr float CMP(float X, float Y)
   {
      return (std::abs(X) - std::abs(Y)) <= FLT_EPSILON ? std::max(1.0f, std::max(std::abs(X), std::abs(Y))) : 0.0f;
   }
}