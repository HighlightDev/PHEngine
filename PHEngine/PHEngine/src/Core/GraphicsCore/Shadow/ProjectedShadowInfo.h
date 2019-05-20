#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Core/GraphicsCore/Shadow/TextureAtlasFactory.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

using namespace Graphics::Texture;

namespace Graphics
{

   struct ProjectedShadowInfo
   {
   private:

      LazyTextureAtlasObtainer m_shadowAtlasCellResource;

      mutable uint32_t m_framebufferDesc;

      void AllocateFramebuffer() const;

      void DeallocateFramebuffer() const;

   public:

      std::vector<glm::mat4x4> ShadowViewMatrices;
      std::vector<glm::mat4x4> ShadowProjectionMatrices;
      glm::mat4 ShadowBiasMatrix;
      glm::vec3 Offset;

      glm::vec4 GetPosOffsetShadowMapAtlas() const;

      std::shared_ptr<ITexture> GetAtlasResource() const;

      void BindShadowFramebuffer(bool clearDepthBuffer) const;

      std::vector<glm::mat4> GetShadowMatrix() const;

      ProjectedShadowInfo(const LazyTextureAtlasObtainer & shadowAtlasCellResource);

      ~ProjectedShadowInfo();
   };

}

