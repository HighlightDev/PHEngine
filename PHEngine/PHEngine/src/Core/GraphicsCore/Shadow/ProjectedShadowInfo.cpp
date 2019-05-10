#include "ProjectedShadowInfo.h"

namespace Graphics
{

   ProjectedShadowInfo::ProjectedShadowInfo(const LazyTextureAtlasObtainer& shadowAtlasCellResource)
      : m_shadowAtlasCellResource(shadowAtlasCellResource)
      , ShadowBiasMatrix(0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0.5f, 0.5f, 0.5f, 1)
   {
      m_framebufferDesc = std::numeric_limits<uint32_t>::max();
      ShadowProjectionMatrices.push_back(glm::mat4(1));
      ShadowViewMatrices.push_back(glm::mat4(1));
      auto resourceObtainer = m_shadowAtlasCellResource.GetTextureAtlasCellResource();
      if (resourceObtainer)
      {
         AllocateFramebuffer();
      }
   }

   ProjectedShadowInfo::~ProjectedShadowInfo()
   {
      DeallocateFramebuffer();
   }

   void ProjectedShadowInfo::AllocateFramebuffer() const
   {
      glGenFramebuffers(1, &m_framebufferDesc);

      glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferDesc);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowAtlasCellResource.GetTextureAtlasCellResource()->GetAtlasResource()->GetTextureDescriptor(), 0);
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   void ProjectedShadowInfo::DeallocateFramebuffer() const
   {
      glDeleteFramebuffers(1, &m_framebufferDesc);
   }

   void ProjectedShadowInfo::BindShadowFramebuffer() const
   {
      if (m_framebufferDesc == std::numeric_limits<uint32_t>::max()) // not allocated framebuffer
      {
         AllocateFramebuffer();
      }

      glm::ivec2 rezolution = m_shadowAtlasCellResource.GetTextureAtlasCellResource()->GetAtlasResource()->GetTextureRezolution();
      glViewport(0, 0, rezolution.x, rezolution.y);
      glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferDesc);
      glClear(GL_DEPTH_BUFFER_BIT);
      glDrawBuffer(GL_NONE);
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
