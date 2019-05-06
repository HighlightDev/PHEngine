#include "ProjectedShadowInfo.h"

namespace Graphics
{

   ProjectedShadowInfo::ProjectedShadowInfo(const TextureAtlasCell& shadowAtlasCell)
      : m_shadowAtlasCell(shadowAtlasCell)
   {
   }

   ProjectedShadowInfo::~ProjectedShadowInfo()
   {
   }

   glm::vec4 ProjectedShadowInfo::GetPosOffsetShadowMapAtlas() const
   {
      float x_start = (float)m_shadowAtlasCell.X / (float)m_shadowAtlasCell.TotalShadowMapWidth;
      float y_start = (float)m_shadowAtlasCell.Y / (float)m_shadowAtlasCell.TotalShadowMapHeight;
      float x_offset = (float)m_shadowAtlasCell.Width / (float)m_shadowAtlasCell.TotalShadowMapWidth;
      float y_offset = (float)m_shadowAtlasCell.Height / (float)m_shadowAtlasCell.TotalShadowMapHeight;

      return glm::vec4(x_start, y_start, x_offset, y_offset);
   }

}
