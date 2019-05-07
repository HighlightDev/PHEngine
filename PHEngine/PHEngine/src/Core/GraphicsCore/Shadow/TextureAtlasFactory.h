#pragma once
#include "Core/GraphicsCore/Texture/TexParams.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

#include <memory>
#include <map>
#include <stdint.h>
#include <glm/vec2.hpp>
#include <vector>

using namespace Graphics::Texture;

namespace Graphics
{

   struct TextureAtlasCell
   {
      int32_t TotalShadowMapHeight;
      int32_t TotalShadowMapWidth;

      int32_t X;
      int32_t Y;
      int32_t Width;
      int32_t Height;

      TextureAtlasCell(int32_t totalShadowMapHeight, int32_t totalShadowMapWidth, int32_t x, int32_t y, int32_t width, int32_t height)
         : TotalShadowMapHeight(totalShadowMapHeight)
         , TotalShadowMapWidth(totalShadowMapWidth)
         , X(x)
         , Y(y)
         , Width(width)
         , Height(height)
      {
      }

      TextureAtlasCell() = default;

      inline int32_t GetSquareValue() const {

         return Width * Height;
      }

      bool operator==(const TextureAtlasCell& cell) const {

         return cell.X == this->X && cell.Y == this->Y &&
            cell.Width == this->Width && cell.Height == this->Height &&
            cell.TotalShadowMapHeight == this->TotalShadowMapHeight &&
            cell.TotalShadowMapWidth == this->TotalShadowMapWidth;
      }
   };

   struct TextureAtlasCellResource
   {
   private:

      TextureAtlasCell m_atlasCell;

      std::shared_ptr<ITexture> m_atlasResource;

   public:

      TextureAtlasCellResource(const TextureAtlasCell& cell, const std::shared_ptr<ITexture>& resource)
         : m_atlasCell(cell)
         , m_atlasResource(resource)
      {
      }

      inline TextureAtlasCell GetAtlasCell() const
      {
         return m_atlasCell;
      }

      inline std::shared_ptr<ITexture> GetAtlasResource() const
      {
         return m_atlasResource;
      }
   };

   class TextureAtlas
   {
      friend class TextureAtlasFactory;

      int32_t shadow_map_size;

      std::map<size_t, TextureAtlasCell> Cells;

      std::shared_ptr<ITexture> m_atlasTexture;

      /* Flag is true when memory for shadowmap was already allocated,
   to reuse space, you should first deallocate possessed memory space */
      bool bInvalidated = false;

      void AllocateReservedMemory();

      void DeallocateMemory();

      void ShrinkReservedMemory();
   };

   class LazyTextureAtlasObtainer;

   class TextureAtlasFactory
   {
      friend class LazyTextureAtlasObtainer;

      static std::unique_ptr<TextureAtlasFactory> m_instance;

      std::vector<std::shared_ptr<TextureAtlas>> m_textureAtlases;

      std::vector<std::pair<size_t, glm::ivec2>> Reservations;

   public:

      enum { SHADOW_MAP_SIZE = 1 << 13 };

      TextureAtlasFactory();

      ~TextureAtlasFactory();

      static std::unique_ptr<TextureAtlasFactory>& GetInstance()
      {
         if (!m_instance)
            m_instance = std::make_unique<TextureAtlasFactory>();

         return m_instance;
      }

      void AllocateTextureAtlasSpace();

      LazyTextureAtlasObtainer AddTextureAtlasRequest(glm::ivec2 size);

   private:

      std::shared_ptr<TextureAtlasCellResource> GetTextureAtlasCellByRequestId(size_t requestId);

      void AddTextureAtlasReservation(size_t requestId, glm::ivec2 size);

      void SplitChunk(std::vector<TextureAtlasCell>& emptyChunks, std::vector<TextureAtlasCell>::const_iterator splittingEmptyChunkIt, TextureAtlasCell& splitCenterCell);
   };

   class LazyTextureAtlasObtainer
   {
      static size_t m_requestId;

   public:

      size_t MyRequestId;

      LazyTextureAtlasObtainer();

      std::shared_ptr<TextureAtlasCellResource> GetTextureAtlasCellResource();
   };

}

