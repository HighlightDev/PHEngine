#pragma once

namespace Game
{
   namespace ComponentType
   {
      #define COMPONENT 0x000000001  // a
      #define SCENE_COMPONENT 0x000000011 //b

      #define COLLISION_COMPONENT (SCENE_COMPONENT | 0x000001000)
      #define LIGHT_COMPONENT (SCENE_COMPONENT | 0x000010000)
      #define DIR_LIGHT_COMPONENT (LIGHT_COMPONENT| 0x000100000)
      #define POINT_LIGHT_COMPONENT (LIGHT_COMPONENT | 0x001000000)

      #define PRIMITIVE_COMPONENT (SCENE_COMPONENT        | 0x000000100)
      #define SKYBOX_COMPONENT (PRIMITIVE_COMPONENT      | 0x000001000)
      #define STATIC_MESH_COMPONENT (PRIMITIVE_COMPONENT | 0x000010000)
      #define WATER_PLANE_COMPONENT (PRIMITIVE_COMPONENT | 0x010000000)
      #define SKELETAL_MESH_COMPONENT (PRIMITIVE_COMPONENT | 0x10000000000)

      #define INPUT_COMPONENT (COMPONENT | 0x100000000)
      #define MOVEMENT_COMPONENT (COMPONENT | 0x1000000000)
	}
}
