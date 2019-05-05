#pragma once

#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <glm/vec2.hpp>
#include <vector>

namespace Graphics
{

   struct ShadowMapAtlasCell
   {

      int32_t TotalShadowMapHeight;
      int32_t TotalShadowMapWidth;

      int32_t X;
      int32_t Y;
      int32_t Width;
      int32_t Height;

      ShadowMapAtlasCell(int32_t totalShadowMapHeight, int32_t totalShadowMapWidth, int32_t x, int32_t y, int32_t width, int32_t height)
         : TotalShadowMapHeight(totalShadowMapHeight)
         , TotalShadowMapWidth(totalShadowMapWidth)
         , X(x)
         , Y(y)
         , Width(width)
         , Height(height)
      {
      }

      ShadowMapAtlasCell() = default;

      inline int32_t GetSquareValue() const {

         return Width * Height;
      }

      bool operator==(const ShadowMapAtlasCell& cell) const {

         return cell.X == this->X && cell.Y == this->Y &&
            cell.Width == this->Width && cell.Height == this->Height &&
            cell.TotalShadowMapHeight == this->TotalShadowMapHeight &&
            cell.TotalShadowMapWidth == this->TotalShadowMapWidth;
      }
   };

   struct TextureAtlas
   {

   private:

      int32_t shadow_map_size;

   public:
      std::vector<ShadowMapAtlasCell> Cells;

      /* Flag is true when memory for shadowmap was already allocated,
         to reuse space, you should first deallocate possessed memory space */
      bool bInvalidated = false;

      void AllocateReservedMemory();

      void DeallocateMemory();

      int32_t PushShadowMapSpace(glm::ivec2 size);

   private:

      void ShrinkReservedMemory();
   };

   class LazyTextureAtlasObtainer;

   class TextureAtlasFactory
   {

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

      void ReserveShadowMapSpace();

      LazyTextureAtlasObtainer AddTextureAtlasRequest(glm::ivec2 size);

      std::shared_ptr<TextureAtlas> GetTextureAtlasByRequestId(size_t requestId);

   private:

      void AddTextureAtlasReservation(size_t requestId, glm::ivec2 size);

      void SplitChunk(std::vector<ShadowMapAtlasCell>& emptyChunks, std::vector<ShadowMapAtlasCell>::const_iterator splittingEmptyChunkIt, ShadowMapAtlasCell& splitCenterCell);
   };

   class LazyTextureAtlasObtainer
   {
      static size_t m_requestId;

   public:

      size_t MyRequestId;

      LazyTextureAtlasObtainer();

      std::shared_ptr<TextureAtlas> GetTextureAtlas()
      {
         return TextureAtlasFactory::GetInstance()->GetTextureAtlasByRequestId(m_requestId);
      }
   };

}

