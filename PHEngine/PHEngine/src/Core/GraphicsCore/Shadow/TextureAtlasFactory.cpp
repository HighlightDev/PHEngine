#include "TextureAtlasFactory.h"
#include "Core/ResourceManagerCore/Pool/RenderTargetPool.h"

#include <algorithm>
#include <GL/glew.h>

using namespace Resources;

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

   void TextureAtlasFactory::AddTextureAtlasCubemapReservation(size_t requestId, glm::ivec2 size) 
   {
      if (Reservations.size() == 0)
      {
         for (size_t index = 0; index < 6; index++)
            Reservations.emplace_back(std::make_pair(requestId, std::vector<glm::ivec2>({ size })));
      }
      else
      {
         const auto calcSquare = [](const glm::ivec2& quad) -> int32_t {  return (quad.x * quad.y); };
         bool bInserted = false;

         // Inserting new reservation size according to ascending rule
         int32_t index = 0;
         size_t reservationsCount = Reservations.size();

         for (auto cit = Reservations.cbegin(); cit != Reservations.end(); ++cit, index++)
         {
            std::vector<glm::ivec2> reservationPack = cit->second;
            // Cube map reservation
            {
               auto& reservation = reservationPack[0];
               if (calcSquare((reservation)) <= calcSquare(size))
               {
                  Reservations.insert(cit, std::make_pair(requestId, std::vector<glm::ivec2>({ size })));
                  bInserted = true;
                  break;
               }
            }
         }

         // If new reservation size is the lowest - just insert it in the end
         if (!bInserted)
         {
            Reservations.emplace_back(std::make_pair(requestId, std::vector<glm::ivec2>({ size })));
         }
      }
   }

   void TextureAtlasFactory::AddTextureAtlasReservation(size_t requestId, glm::ivec2 size)
   {
      if (Reservations.size() == 0)
      {
         Reservations.emplace_back(std::make_pair(requestId, std::vector<glm::ivec2>({ size })));
      }
      else
      {
         const auto calcSquare = [](const glm::ivec2& quad) -> int32_t {  return (quad.x * quad.y); };
         bool bInserted = false;

         // Inserting new reservation size according to ascending rule
         int32_t index = 0;
         for (auto cit = Reservations.cbegin(); cit != Reservations.end(); ++cit, index++)
         {
            if (calcSquare((cit->second[0])) <= calcSquare(size))
            {
               Reservations.insert(cit, std::make_pair(requestId, std::vector<glm::ivec2>({ size })));
               bInserted = true;
               break;
            }
         }

         // If new reservation size is the lowest - just insert it in the end
         if (!bInserted)
         {
            Reservations.emplace_back(std::make_pair(requestId, std::vector<glm::ivec2>({ size })));
         }
      }
   }

   LazyTextureAtlasObtainer TextureAtlasFactory::AddTextureAtlasRequestCubemap(glm::ivec2 size)
   {
      LazyTextureAtlasObtainer obtainer;
      AddTextureAtlasCubemapReservation(obtainer.MyRequestId, size);
      return obtainer;
   }

   LazyTextureAtlasObtainer TextureAtlasFactory::AddTextureAtlasRequest(glm::ivec2 size)
   {
      LazyTextureAtlasObtainer obtainer;
      AddTextureAtlasReservation(obtainer.MyRequestId, size);
      return obtainer;
   }

   void TextureAtlasFactory::AllocateTextureAtlasSpace()
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
         std::map<size_t, std::vector<TextureAtlasCell>>& cells = atlas.Cells;
         std::vector<TextureAtlasCell> emptyChunks = { TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE) };

         auto it = Reservations.begin();
         while (it != Reservations.end())
         {
            auto& reservationPack = it->second;
            if (reservationPack.size() == 1) // 2D texture
            {
               auto& size = reservationPack[0];
               auto relevantChunkIt = getRelevantEmptyChunk(emptyChunks, size);
               if (relevantChunkIt != emptyChunks.end())
               {
                  TextureAtlasCell newCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, relevantChunkIt->X, relevantChunkIt->Y, size.x, size.y);
                  cells.insert(std::make_pair(it->first, std::vector<TextureAtlasCell>({ newCell })));
                  SplitChunk(emptyChunks, relevantChunkIt, newCell);

                  it = Reservations.erase(it);
               }
               else
               {
                  ++it;
               }
            }
            else  // Cube map texture
            {

            }
         }
         if (atlas.Cells.size())
         {
            atlas.ShrinkReservedMemory();
            atlas.AllocateReservedMemory();
            m_textureAtlases.push_back(std::make_shared<TextureAtlas>(atlas));
         }
      }

      Reservations.clear();
   }

   std::shared_ptr<TextureAtlasCellResource> TextureAtlasFactory::GetTextureAtlasCellByRequestId(size_t requestId)
   {
      std::shared_ptr<TextureAtlasCellResource> result;

      for (auto& atlas : m_textureAtlases)
      {
         std::map<size_t, std::vector<TextureAtlasCell>>::const_iterator it = atlas->Cells.find(requestId);
         if (it != atlas->Cells.end())
         {
            result = std::make_shared<TextureAtlasCellResource>(it->second, atlas->m_atlasTexture);
            break;
         }
      }

      return result;
   }

   void TextureAtlas::AllocateReservedMemory()
   {
      if (!m_atlasTexture)
      {
         TexParams shadowMapParams(shadow_map_size, shadow_map_size, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, 0, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_REPEAT);
         m_atlasTexture = RenderTargetPool::GetInstance()->GetOrAllocateResource<Texture2d>(shadowMapParams);
      }
   }

   void TextureAtlas::DeallocateMemory()
   {
      if (!m_atlasTexture)
      {
         RenderTargetPool::GetInstance()->TryToFreeMemory(m_atlasTexture);
         m_atlasTexture = std::shared_ptr<ITexture>(nullptr);
      }
   }

   void TextureAtlas::ShrinkReservedMemory()
   {
      glm::ivec2 max(0);

      for (auto cit = Cells.cbegin(); cit != Cells.cend(); ++cit)
      {
         const auto& cell = cit->second[0];
         max.x = std::max(cell.X + cell.Width, max.x);
         max.y = std::max(cell.Y + cell.Height, max.y);
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

      for (auto it = Cells.begin(); it != Cells.end(); ++it)
      {
         std::vector<TextureAtlasCell>& cellPack = it->second;
         for (auto& cell : cellPack)
         {
            cell.TotalShadowMapWidth = shadow_map_size;
            cell.TotalShadowMapHeight = shadow_map_size;
         } 
      }
   }

   void TextureAtlasFactory::SplitChunk(std::vector<TextureAtlasCell>& emptyChunks, std::vector<TextureAtlasCell>::const_iterator splittingEmptyChunkIt, TextureAtlasCell& splitCenterCell)
   {
      TextureAtlasCell leftTopCell = TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, splittingEmptyChunkIt->X, splittingEmptyChunkIt->Y + splitCenterCell.Height, splitCenterCell.Width, splittingEmptyChunkIt->Height - splitCenterCell.Height);
      TextureAtlasCell rightBottomCell = TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, splittingEmptyChunkIt->X + splitCenterCell.Width, splittingEmptyChunkIt->Y, splittingEmptyChunkIt->Width - splitCenterCell.Width, splitCenterCell.Height);
      TextureAtlasCell rightTopCell = TextureAtlasCell(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, splittingEmptyChunkIt->X + splitCenterCell.Width, splittingEmptyChunkIt->Y + splitCenterCell.Height, splittingEmptyChunkIt->Width - splitCenterCell.Width, splittingEmptyChunkIt->Height - splitCenterCell.Height);

      // Remove splitting empty chunk because it was split
      emptyChunks.erase(splittingEmptyChunkIt);

      auto sortFunctor = [](const TextureAtlasCell& atlasCell1, const TextureAtlasCell& atlasCell2) -> bool
      {
         int32_t atlas1Square = atlasCell1.Height * atlasCell1.Width;
         int32_t atlas2Square = atlasCell2.Height * atlasCell2.Width;
         if (atlas1Square > atlas2Square)
         {
            return true;
         }
         else if (atlas1Square == atlas2Square)
         {
            return (atlasCell1.Height > atlasCell2.Height);
         }
         else return false;
      };

      emptyChunks.emplace_back(leftTopCell);
      emptyChunks.emplace_back(rightTopCell);
      emptyChunks.emplace_back(rightBottomCell);

      std::sort(emptyChunks.begin(), emptyChunks.end(), sortFunctor);
   }

   std::shared_ptr<TextureAtlasCellResource> LazyTextureAtlasObtainer::GetTextureAtlasCellResource() const
   {
      return TextureAtlasFactory::GetInstance()->GetTextureAtlasCellByRequestId(MyRequestId);
   }

}
