#include "ProjectedShadowInfo.h"

namespace Graphics
{

   ProjectedShadowInfo::ProjectedShadowInfo(const LazyTextureAtlasObtainer& shadowAtlasCellResource)
      : m_shadowAtlasCellResource(shadowAtlasCellResource)
      , ShadowBiasMatrix(0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0.5f, 0.5f, 0.5f, 1)
   {
      ShadowProjectionMatrices.push_back(glm::mat4(1));
      ShadowViewMatrices.push_back(glm::mat4(1));
   }

   ProjectedShadowInfo::~ProjectedShadowInfo()
   {
   }

   std::shared_ptr<ITexture> ProjectedShadowInfo::GetAtlasResource() const {

      std::shared_ptr<ITexture> result;
      auto resourceObtainer = m_shadowAtlasCellResource.GetTextureAtlasCellResource();
      if (resourceObtainer)
      {
         result = resourceObtainer->GetAtlasResource();
      }
      return result;
   }

   glm::vec4 ProjectedShadowInfo::GetPosOffsetShadowMapAtlas() const
   {
      glm::vec4 resultOffset;

      auto resourceObtainer = m_shadowAtlasCellResource.GetTextureAtlasCellResource();
      if (resourceObtainer)
      {
         float x_start = (float)resourceObtainer->GetAtlasCell().X / (float)resourceObtainer->GetAtlasCell().TotalShadowMapWidth;
         float y_start = (float)resourceObtainer->GetAtlasCell().Y / (float)resourceObtainer->GetAtlasCell().TotalShadowMapHeight;
         float x_offset = (float)resourceObtainer->GetAtlasCell().Width / (float)resourceObtainer->GetAtlasCell().TotalShadowMapWidth;
         float y_offset = (float)resourceObtainer->GetAtlasCell().Height / (float)resourceObtainer->GetAtlasCell().TotalShadowMapHeight;

         resultOffset = glm::vec4(x_start, y_start, x_offset, y_offset);
      }

      return resultOffset;
   }

}
