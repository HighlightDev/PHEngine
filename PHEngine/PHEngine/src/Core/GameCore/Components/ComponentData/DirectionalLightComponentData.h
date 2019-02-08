#pragma once

#include "Core/GameCore/Components/ComponentType.h"
#include "LightComponentData.h"

namespace Game
{

	struct DirectionalLightComponentData : 
      public LightComponentData
	{

      DirectionalLightComponentData(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
         : LightComponentData(ambient, diffuse, specular)
         , Direction(direction)
         , Translation(translation)
         , Rotation(rotation)
         , Scale(scale)
      {
      }

      virtual ComponentType GetType() override {

         return DIR_LIGHT_COMPONENT;
      }

      glm::vec3 Direction;
      glm::vec3 Translation;
      glm::vec3 Rotation;
      glm::vec3 Scale;
	};

}