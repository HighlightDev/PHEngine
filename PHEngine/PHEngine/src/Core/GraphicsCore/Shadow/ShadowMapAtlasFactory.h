#pragma once

#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <vector>
#include <glm/vec2.hpp>

namespace Graphics
{

   struct ShadowMapAtlasCell
   {
      int32_t X;
      int32_t Y;
      int32_t Width;
      int32_t Height;

      ShadowMapAtlasCell(int32_t x, int32_t y, int32_t width, int32_t height)
         : X(x)
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

         return cell.X == this->X && cell.Y == this->Y && cell.Width == this->Width && cell.Height == this->Height;
      }
   };

   struct ShadowMapAtlas
   {

   private:

      int32_t shadow_map_size;

   public:
      std::vector<ShadowMapAtlasCell> Cells;

      std::vector<glm::ivec2> Reservations;

      /* Flag is true when memory for shadowmap was already allocated,
         to reuse space, you should first deallocate possessed memory space */
      bool bInvalidated = false;

      void AllocateReservedMemory();

      void DeallocateMemory();

      void PushShadowMapSpace(glm::ivec2 size);

   private:

      void ShrinkReservedMemory();
   };

   class ShadowMapAtlasFactory
   {

      static std::unique_ptr<ShadowMapAtlasFactory> m_instance;

   public:

      enum { SHADOW_MAP_SIZE = 1 << 13 };

      ShadowMapAtlasFactory();

      ~ShadowMapAtlasFactory();

      static std::unique_ptr<ShadowMapAtlasFactory>& GetInstance()
      {
         if (!m_instance)
            m_instance = std::make_unique<ShadowMapAtlasFactory>();

         return m_instance;
      }

      void ReserveShadowMapSpace(ShadowMapAtlas& atlas);

   private:

      void SplitChunk(std::vector<ShadowMapAtlasCell>& emptyChunks, std::vector<ShadowMapAtlasCell>::iterator splittingEmptyChunkIt, ShadowMapAtlasCell& splitCenterCell);
   };

}

