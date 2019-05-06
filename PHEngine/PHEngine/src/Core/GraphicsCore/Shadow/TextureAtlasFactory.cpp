#include "TextureAtlasFactory.h"

#include <algorithm>
#include <GL/glew.h>

namespace Graphics
{
   size_t LazyTextureAtlasObtainer::m_requestId = 0;

   LazyTextureAtlasObtainer::LazyTextureAtlasObtainer()
   {
      LazyTextureAtlasObtainer::m_requestId += 1;
      MyRequestId = m_requestId;
   }

   std::unique_ptr<TextureAtlasFactory> TextureAtlasFactory::m_instance;

   TextureAtlasFactory::TextureAtlasFactory()
   {
   }

   TextureAtlasFactory::~TextureAtlasFactory()
   {
   }

   void TextureAtlasFactory::AddTextureAtlasReservation(size_t requestId, glm::ivec2 size)
   {
      if (Reservations.size() == 0)
      {
         Reservations.emplace_back(std::make_pair(requestId, size));
      }
      else
      {
         const auto calcSquare = [](const glm::ivec2& quad) -> int32_t {  return (quad.x * quad.y); };
         bool bInserted = false;

         // Inserting new reservation size according to ascending rule
         int32_t index = 0;
         for (auto cit = Reservations.cbegin(); cit != Reservations.end(); ++cit, index++)
         {
            if (calcSquare((cit->second)) <= calcSquare(size))
            {
               Reservations.insert(cit, std::make_pair(cit->first, size));
               bInserted = true;
               break;
            }
         }

         // If new reservation size is the lowest - just insert it in the end
         if (!bInserted)
         {
            Reservations.emplace_back(std::make_pair(requestId, size));
         }
      }
   }

   LazyTextureAtlasObtainer TextureAtlasFactory::AddTextureAtlasRequest(glm::ivec2 size)
   {
      LazyTextureAtlasObtainer obtainer;
      AddTextureAtlasReservation(obtainer.MyRequestId, size);
      return obtainer;
   }

   void TextureAtlasFactory::ReserveShadowMapSpace()
   {
      auto getRelevantEmptyChunk = [](const std::vector<TextureAtlasCell>& emptyChunks, const glm::ivec2& reservation)
      {
         size_t reverseIndex = emptyChunks.size() - 1;
         std::vector<TextureAtlasCell>::const_iterator result = emptyChunks.end();
         for (auto rit = emptyChunks.rbegin(); rit != emptyChunks.rend(); ++rit, --reverseIndex)
         {
            if (rit->Width >= reservation.x && rit->Height >= reservation.y)
            {
               // enough space for cell in empty chunk
               result = emptyChunks.begin() + reverseIndex;
               break;
            }
         }

         return result;
      };

      while (Reservations.size())
      {
         TextureAtlas atlas;
         std::map<size_t, TextureAtlasCell>& cells = atlas.Cells;
         std::vector<TextureAtlasCell> emptyChunks = { TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE) };
         for (std::vector<std::pair<size_t, glm::ivec2>>::iterator it = Reservations.begin(); it != Reservations.end(); ++it)
         {
            auto relevantChunkIt = getRelevantEmptyChunk(emptyChunks, it->second);
            if (relevantChunkIt != emptyChunks.end())
            {
               TextureAtlasCell newCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, relevantChunkIt->X, relevantChunkIt->Y, it->second.x, it->second.y);
               cells.emplace(std::make_pair(it->first, newCell));
               SplitChunk(emptyChunks, relevantChunkIt, newCell);
            }
         }
         m_textureAtlases.push_back(std::make_shared<TextureAtlas>(atlas));
      }

      Reservations.clear();
   }

   std::shared_ptr<TextureAtlasCell> TextureAtlasFactory::GetTextureAtlasCellByRequestId(size_t requestId)
   {
      std::shared_ptr<TextureAtlasCell> result;

      for (auto& atlas : m_textureAtlases)
      {
         std::map<size_t, TextureAtlasCell>::const_iterator it = atlas->Cells.find(requestId);
         if (it != atlas->Cells.end())
         {
            result = std::make_shared<TextureAtlasCell>(it->second);
            break;
         }
      }

      return result;
   }

   void TextureAtlas::AllocateReservedMemory()
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

   void TextureAtlas::DeallocateMemory()
   {

   }

   void TextureAtlas::ShrinkReservedMemory()
   {
      glm::ivec2 max(0);

      for (auto cit = Cells.cbegin(); cit != Cells.cend(); ++cit)
      {
         max.x = std::max(cit->second.X + cit->second.Width, max.x);
         max.y = std::max(cit->second.Y + cit->second.Height, max.y);
      }

      int32_t shadowMapSize = TextureAtlasFactory::SHADOW_MAP_SIZE;
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
         else
            break;
      }

      shadow_map_size = shadowMapSize;
   }

   void TextureAtlasFactory::SplitChunk(std::vector<TextureAtlasCell>& emptyChunks, std::vector<TextureAtlasCell>::const_iterator splittingEmptyChunkIt, TextureAtlasCell& splitCenterCell)
   {
      TextureAtlasCell leftBottomCell = TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, splittingEmptyChunkIt->X, splittingEmptyChunkIt->Y, splitCenterCell.Width, splittingEmptyChunkIt->Height - splitCenterCell.Height);
      TextureAtlasCell rightBottomCell = TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, splittingEmptyChunkIt->X + splitCenterCell.Width, splittingEmptyChunkIt->Y, splittingEmptyChunkIt->Width - splitCenterCell.Width, splittingEmptyChunkIt->Height - splitCenterCell.Height);
      TextureAtlasCell rightTopCell = TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, splittingEmptyChunkIt->X + splitCenterCell.Width, splittingEmptyChunkIt->Y + splitCenterCell.Height, splittingEmptyChunkIt->Width - splitCenterCell.Width, splitCenterCell.Height);

      // Remove splitting empty chunk because it was split
      emptyChunks.erase(splittingEmptyChunkIt);

      auto sortFunctor = [](const TextureAtlasCell& atlasCell1, const TextureAtlasCell& atlasCell2) -> bool
      {
         return ((atlasCell1.Height * atlasCell1.Width) > (atlasCell2.Height * atlasCell2.Width));
      };

      emptyChunks.emplace_back(leftBottomCell);
      emptyChunks.emplace_back(rightTopCell);
      emptyChunks.emplace_back(rightBottomCell);

      std::sort(emptyChunks.begin(), emptyChunks.end(), sortFunctor);
   }

   std::shared_ptr<TextureAtlasCell> LazyTextureAtlasObtainer::GetTextureAtlasCell()
   {
      return TextureAtlasFactory::GetInstance()->GetTextureAtlasCellByRequestId(m_requestId);
   }

}
