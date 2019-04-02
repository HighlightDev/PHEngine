#include "ShadowMapAtlasFactory.h"

#include <algorithm>
#include <GL/glew.h>

namespace Graphics
{

   std::unique_ptr<ShadowMapAtlasFactory> ShadowMapAtlasFactory::m_instance;

   ShadowMapAtlasFactory::ShadowMapAtlasFactory()
   {
   }

   ShadowMapAtlasFactory::~ShadowMapAtlasFactory()
   {
   }

   void ShadowMapAtlasFactory::ReserveShadowMapSpace(ShadowMapAtlas& atlas)
   {
      std::vector<ShadowMapAtlasCell>& cells = atlas.Cells;
      std::vector<glm::ivec2>& reservations = atlas.Reservations;

      std::vector<ShadowMapAtlasCell> emptyChunks = { ShadowMapAtlasCell(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE) };

      for (std::vector<glm::ivec2>::const_iterator it = reservations.cbegin(); it != reservations.cend(); ++it)
      {
         // If atlas is empty - just push at the beginning
         if (cells.size() == 0)
         {
            ShadowMapAtlasCell newCell(0, 0, it->x, it->y);

            cells.push_back(newCell);
            auto firstEmptyChunkIt = emptyChunks.begin();
            SplitChunk(emptyChunks, firstEmptyChunkIt, newCell);
         }
         else
         {
            for (std::vector<ShadowMapAtlasCell>::reverse_iterator rit = emptyChunks.rbegin(); rit != emptyChunks.rend(); ++rit)
            {
               if (rit->GetSquareValue() >= (it->x * it->y))
               {
                  // enough space for cell in empty chunk

                  ShadowMapAtlasCell newCell(rit->X, rit->Y, it->x, it->y);

                  cells.push_back(newCell);
                  auto emptuChunkIt = std::find(emptyChunks.begin(), emptyChunks.end(), *rit);
                  SplitChunk(emptyChunks, emptuChunkIt, newCell);
                  break;
               }
            }
         }
      }

      reservations.clear();
   }

   void ShadowMapAtlasFactory::SplitChunk(std::vector<ShadowMapAtlasCell>& emptyChunks, std::vector<ShadowMapAtlasCell>::iterator splittingEmptyChunkIt, ShadowMapAtlasCell& splitCenterCell)
   {
      ShadowMapAtlasCell leftBottomCell = ShadowMapAtlasCell(splittingEmptyChunkIt->X, splittingEmptyChunkIt->Y, splitCenterCell.Width, splittingEmptyChunkIt->Height - splitCenterCell.Height);
      ShadowMapAtlasCell rightBottomCell = ShadowMapAtlasCell(splittingEmptyChunkIt->X + splitCenterCell.Width, splittingEmptyChunkIt->Y, splittingEmptyChunkIt->Width - splitCenterCell.Width, splittingEmptyChunkIt->Height - splitCenterCell.Height);
      ShadowMapAtlasCell rightTopCell = ShadowMapAtlasCell(splittingEmptyChunkIt->X + splitCenterCell.Width, splittingEmptyChunkIt->Y + splitCenterCell.Height, splittingEmptyChunkIt->Width - splitCenterCell.Width, splitCenterCell.Height);

      // Remove splitting empty chunk because it was split
      emptyChunks.erase(splittingEmptyChunkIt);

      auto sortFunctor = [](const ShadowMapAtlasCell& atlasCell1, const ShadowMapAtlasCell& atlasCell2) -> bool
      {
         return ((atlasCell1.Height * atlasCell1.Width) > (atlasCell2.Height * atlasCell2.Width));
      };

      emptyChunks.emplace_back(leftBottomCell);
      emptyChunks.emplace_back(rightTopCell);
      emptyChunks.emplace_back(rightBottomCell);

      std::sort(emptyChunks.begin(), emptyChunks.end(), sortFunctor);
   }

   void ShadowMapAtlas::PushShadowMapSpace(glm::ivec2 size)
   {
      if (bInvalidated)
         return;

      if (Reservations.size() == 0)
         Reservations.push_back(size);
      else
      {

         const auto calcSquare = [](const glm::ivec2& quad) -> int32_t {  return (quad.x * quad.y); };
         bool bInserted = false;

         // Inserting new reservation size according to ascending rule
         for (auto cit = Reservations.cbegin(); cit != Reservations.end(); ++cit)
         {
            if (calcSquare((*cit)) <= calcSquare(size))
            {
               Reservations.insert(cit, size);
               bInserted = true;
               break;
            }
         }

         // If new reservation size is the lowest - just insert it in the end
         if (!bInserted)
            Reservations.push_back(size);
      }
   }

   void ShadowMapAtlas::AllocateReservedMemory()
   {
      ShrinkReservedMemory();

      //glGenFramebuffers(1, &m_gBufferFBO);
      //glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFBO);

      //// Depth texture
      //glGenTextures(1, &m_depthBuffer);
      //glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
      //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_windowWidth, m_windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
   }

   void ShadowMapAtlas::DeallocateMemory()
   {

   }

   void ShadowMapAtlas::ShrinkReservedMemory()
   {
      glm::ivec2 max(0);

      for (auto cit = Cells.cbegin(); cit != Cells.cend(); ++cit)
      {
         max.x = std::max(cit->X + cit->Width, max.x);
         max.y = std::max(cit->Y + cit->Height, max.y);
      }

      int32_t shadowMapSize = ShadowMapAtlasFactory::SHADOW_MAP_SIZE;
      while (true)
      {
         if (shadowMapSize > max.x && shadowMapSize > max.y)
         {
            int32_t shrinkedShadowMapSize = shadowMapSize >> 1;
            if (shrinkedShadowMapSize >= max.x && shrinkedShadowMapSize >= max.y)
               shadowMapSize = shrinkedShadowMapSize;
            else
               break;
         }
      }

      shadow_map_size = shadowMapSize;
   }

}