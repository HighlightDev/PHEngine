#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Core/GraphicsCore/Shadow/TextureAtlasFactory.h"

namespace Graphics
{

   struct ProjectedShadowInfo
   {
   private:

      TextureAtlasCell m_shadowAtlasCell;

   public:

      std::vector<glm::mat4x4> ShadowViewMatrices;
      std::vector<glm::mat4x4> ShadowProjectionMatrices;
      glm::vec3 Offset;

      glm::vec4 GetPosOffsetShadowMapAtlas() const;

      ProjectedShadowInfo(const TextureAtlasCell& shadowAtlasCell);

      ~ProjectedShadowInfo();
   };

}

