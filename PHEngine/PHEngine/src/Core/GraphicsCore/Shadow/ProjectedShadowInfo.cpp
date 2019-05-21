#include "ProjectedShadowInfo.h"

namespace Graphics
{

   ProjectedShadowInfo::ProjectedShadowInfo(const LazyTextureAtlasObtainer& shadowAtlasCellResource)
      : m_shadowAtlasCellResource(shadowAtlasCellResource)
      , m_framebufferDesc(std::numeric_limits<uint32_t>::max())
      , m_shadowFramebuffer(nullptr)
      , ShadowBiasMatrix(
         0.5f, 0, 0, 0,
         0, 0.5f, 0, 0,
         0, 0, 0.5f, 0, 
         0.5f, 0.5f, 0.5f, 1)
      , Offset(0)
   {
      ShadowProjectionMatrices.push_back(glm::mat4(1));
      ShadowViewMatrices.push_back(glm::mat4(1));
   }

   ProjectedShadowInfo::~ProjectedShadowInfo()
   {
      DeallocateFramebuffer();
   }

   void ProjectedShadowInfo::AllocateFramebuffer() const
   {
      m_shadowFramebuffer = std::make_shared<ShadowFramebuffer>(m_shadowAtlasCellResource.GetTextureAtlasCellResource()->GetAtlasResource());
      m_shadowFramebuffer->BindFramebuffer(1);
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);
      m_shadowFramebuffer->UnbindFramebuffer();
   }

   void ProjectedShadowInfo::DeallocateFramebuffer() const
   { 
      m_shadowFramebuffer->CleanUp();
   }

   void ProjectedShadowInfo::BindShadowFramebuffer(bool clearDepthBuffer) const
   {
      if (!m_shadowFramebuffer)
      {
         AllocateFramebuffer();
      }

      auto atlas_cell = m_shadowAtlasCellResource.GetTextureAtlasCellResource()->GetAtlasCell();
      const GLbitfield clearDepthFlag = clearDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0;
      m_shadowFramebuffer->RenderToFBO(1, atlas_cell.X, atlas_cell.Y, atlas_cell.Width, atlas_cell.Height, clearDepthFlag);
      glDrawBuffer(GL_NONE);
   }

   std::vector<glm::mat4> ProjectedShadowInfo::GetShadowMatrix() const
   {
      std::vector<glm::mat4> shadowMatrices;

      for (size_t i = 0; i < ShadowViewMatrices.size(); i++)
      {
         glm::mat4 toShadowSpaceMatrix(1);
         
         toShadowSpaceMatrix *= ShadowBiasMatrix;
         toShadowSpaceMatrix *= ShadowProjectionMatrices[i];
         toShadowSpaceMatrix *= ShadowViewMatrices[i];
         shadowMatrices.push_back(toShadowSpaceMatrix);
      }

      return shadowMatrices;
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
         float x = static_cast<float>(resourceObtainer->GetAtlasCell().X);
         float y = static_cast<float>(resourceObtainer->GetAtlasCell().Y);
         float width = static_cast<float>(resourceObtainer->GetAtlasCell().Width);
         float height = static_cast<float>(resourceObtainer->GetAtlasCell().Height);

         float invShadowMapWidth = 1.0f / static_cast<float>(resourceObtainer->GetAtlasCell().TotalShadowMapWidth);
         float invShadowMapHeight = 1.0f / static_cast<float>(resourceObtainer->GetAtlasCell().TotalShadowMapHeight);

         float x_start = x * invShadowMapWidth;
         float y_start = y * invShadowMapHeight;
         float x_offset = width * invShadowMapWidth;
         float y_offset = height * invShadowMapHeight;

         resultOffset = glm::vec4(x_start, y_start, x_offset, y_offset);
      }

      return resultOffset;
   }

}
