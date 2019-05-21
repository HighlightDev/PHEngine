#include "ShadowFramebuffer.h"

namespace Game
{
   namespace FramebufferImpl
   {

      ShadowFramebuffer::ShadowFramebuffer(sharedTexture_t shadowmapTextureResource)
         : Framebuffer()
         , m_shadowmapTextureResource(shadowmapTextureResource)
      {
         Init();
      }

      ShadowFramebuffer::~ShadowFramebuffer()
      {
      }

      void ShadowFramebuffer::SetTextures()
      {
      }

      void ShadowFramebuffer::SetFramebuffers()
      {
         Base::GenFramebuffers(1);
         Base::BindFramebuffer(1);
         Base::Attach2DTextureToFramebuffer(GL_DEPTH_ATTACHMENT, m_shadowmapTextureResource);
      }

      void ShadowFramebuffer::SetRenderbuffers()
      {
      }

      void ShadowFramebuffer::CleanUp()
      {
         Base::CleanUp();
      }

   }
}
