#pragma once

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <gl/glew.h>

#include "Core/GraphicsCore/Texture/ITexture.h"

using namespace Graphics::Texture;

namespace Graphics
{
   class Framebuffer
   {
   protected:

      std::vector<uint32_t> m_framebufferID;
      std::vector<uint32_t> m_renderbufferID;

      virtual void SetTextures() = 0;
      virtual void SetFramebuffers() = 0;
      virtual void SetRenderbuffers() = 0;

      void GenFramebuffers(size_t countFramebuffers);
      void GenRenderbuffers(size_t countRenderbuffers);
      void BindFramebuffer(size_t framebufferIndex) const;
      void UnbindFramebuffer() const;
      void BindRenderBuffer(size_t renderbufferIndex) const;
      void SetRenderbufferStorage(int32_t renderbufferType, size_t rezolutionWidth, size_t rezolutionHeight) const;
      void SetRenderbufferStorage(int32_t renderbufferType, const glm::ivec2& screenRezolution) const;
      void Attach2DTextureToFramebuffer(int32_t framebufferAttachment, uint32_t attachedTexture) const;
      void Attach2DTextureToFramebuffer(int32_t framebufferAttachment, const std::shared_ptr<ITexture> texture) const;
      void AttachRenderbufferToFramebuffer(int32_t framebufferAttachment, size_t renderbufferIndex) const;
      void AttachRenderbufferToFramebuffer(int32_t framebufferAttachment) const;

      void Init();

   private:

      inline GLenum GetFramebufferErrorCode() const;

      void BindFramebufferImpl(size_t framebufferIndex, size_t viewportWidth, size_t viewportHeight) const;

   public:

      Framebuffer();

      virtual ~Framebuffer();

      std::string GetFramebufferLog() const;
      void RenderToFBO(size_t framebufferIndex, size_t viewportWidth, size_t viewportHeight) const;
      void RenderToFBO(size_t framebufferIndex, const glm::ivec2& viewportRezolution) const;
      virtual void CleanUp();
   };

   inline GLenum Framebuffer::GetFramebufferErrorCode() const
   {
      GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      return result;
   }

}



