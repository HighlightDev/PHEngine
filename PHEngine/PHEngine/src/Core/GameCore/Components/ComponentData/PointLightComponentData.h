#pragma once

#include "Core/GameCore/Components/ComponentType.h"
#include "LightComponentData.h"

namespace Game
{

	struct PointLightComponentData : 
      public LightComponentData
	{

      PointLightComponentData(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, glm::vec3 position, glm::vec3 attenuation, float radianceSqrRadius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, ProjectedShadowInfo* shadowInfo)
         : LightComponentData(ambient, diffuse, specular, shadowInfo)
         , Position(position)
         , Attenuation(attenuation)
         , RadianceSqrRadius(radianceSqrRadius)
         , Translation(translation)
         , Rotation(rotation)
         , Scale(scale)
      {
      }

      virtual uint64_t GetType() override {

         return POINT_LIGHT_COMPONENT;
      }

      glm::vec3 Position;
      glm::vec3 Attenuation;
      float RadianceSqrRadius;
      glm::vec3 Translation;
      glm::vec3 Rotation;
      glm::vec3 Scale;
	};

}