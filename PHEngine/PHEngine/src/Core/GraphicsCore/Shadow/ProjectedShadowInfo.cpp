#include "ProjectedShadowInfo.h"

namespace Graphics
{

   ProjectedShadowInfo::ProjectedShadowInfo(const std::shared_ptr<TextureAtlasCellResource>& shadowAtlasCellResource)
      : m_shadowAtlasCellResource(shadowAtlasCellResource)
   {
   }

   ProjectedShadowInfo::~ProjectedShadowInfo()
   {
   }

   std::shared_ptr<ITexture> ProjectedShadowInfo::GetAtlasResource() const {

      return m_shadowAtlasCellResource->GetAtlasResource();
   }

   glm::vec4 ProjectedShadowInfo::GetPosOffsetShadowMapAtlas() const
   {
      glm::vec4 result;

      if (m_shadowAtlasCellResource)
      {
         float x_start = (float)m_shadowAtlasCellResource->GetAtlasCell().X / (float)m_shadowAtlasCellResource->GetAtlasCell().TotalShadowMapWidth;
         float y_start = (float)m_shadowAtlasCellResource->GetAtlasCell().Y / (float)m_shadowAtlasCellResource->GetAtlasCell().TotalShadowMapHeight;
         float x_offset = (float)m_shadowAtlasCellResource->GetAtlasCell().Width / (float)m_shadowAtlasCellResource->GetAtlasCell().TotalShadowMapWidth;
         float y_offset = (float)m_shadowAtlasCellResource->GetAtlasCell().Height / (float)m_shadowAtlasCellResource->GetAtlasCell().TotalShadowMapHeight;


         result = glm::vec4(x_start, y_start, x_offset, y_offset);
      }
      return result;
   }

}
