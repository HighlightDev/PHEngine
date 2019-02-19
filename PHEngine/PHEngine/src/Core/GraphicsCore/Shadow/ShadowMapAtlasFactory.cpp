#include "ShadowMapAtlasFactory.h"
#include <tuple>


std::unique_ptr<ShadowMapAtlasFactory> ShadowMapAtlasFactory::m_instance;

ShadowMapAtlasFactory::ShadowMapAtlasFactory()
{
}

ShadowMapAtlasFactory::~ShadowMapAtlasFactory()
{
}

void ShadowMapAtlasFactory::PushShadowMapSpace(glm::ivec2 size, ShadowMapAtlas& atlas)
{
   if (atlas.bInvalidated)
      return;

   std::vector<glm::ivec2>& reservation = m_reservations[atlas];
   if (reservation.size() == 0)
      m_reservations[atlas].push_back(size);
   else
   {

      const auto calcSquare = [](const glm::ivec2& quad) -> float {  return quad.x * quad.y; };
      bool bInserted = false;

      // Inserting new reservation size according to ascending rule
      for (auto cit = reservation.cbegin(); cit != reservation.end(); ++cit)
      {
         if (calcSquare((*cit)) <= calcSquare(size))
         {
            reservation.insert(cit, size);
            bInserted = true;
            break;
         }
      }

      // If new reservation size is the lowest - just insert it in the end
      if (!bInserted)
         reservation.push_back(size);
   }
}

void ShadowMapAtlasFactory::ReserveShadowMapSpace(ShadowMapAtlas& atlas)
{
   std::vector<glm::ivec2>& reservation = m_reservations[atlas];
   
   if (reservation.size() <= 0)
      return;

   float sumSquares = 0.0f;

   const auto calcSquare = [](const glm::ivec2& quad) -> float {  return quad.x * quad.y; };
   for (auto cit = reservation.cbegin(); cit != reservation.cend(); ++cit)
   {
      sumSquares += calcSquare(*cit);
   }
  
   int32_t bound_size = std::sqrt(sumSquares);

   int32_t max_square_height = reservation[0].y;
   int32_t max_square_width = reservation[0].x;

   const auto squareIntersects = [&](const ShadowMapAtlasCell& cell) -> std::tuple<bool, ShadowMapAtlasCell>
   {
      bool bIntersects = false;
      ShadowMapAtlasCell out_cell;
      glm::ivec2 max1 = glm::ivec2(cell.X + cell.Width, cell.Y + cell.Height);
      glm::ivec2 min1 = glm::ivec2(cell.X, cell.Y);
      for (auto& iCell : atlas.Cells)
      {
         glm::ivec2 max2 = glm::ivec2(iCell.X + iCell.Width, iCell.Y + iCell.Height);
         glm::ivec2 min2 = glm::ivec2(iCell.X, iCell.Y);
         bool bIntersects = ((min1.x <= max2.x && max1.x >= min2.x) &&
            (min1.y <= max2.y && max1.y >= min2.y));

         if (bIntersects)
         {
            out_cell = iCell;
            break;
         }
      }
      return std::make_tuple(bIntersects, out_cell);
   };

   const auto squareOutOfBounds = [&](const ShadowMapAtlasCell& cell) -> bool
   {
      bool bOutOfBounds = false;
      glm::ivec2 max1 = glm::ivec2(cell.X + cell.Width, cell.Y + cell.Height);

      if (max1.x > bound_size || max1.y > bound_size)
         bOutOfBounds = true;

      return bOutOfBounds;
   };
   
   for (auto it = reservation.begin(); it != reservation.end(); ++it)
   {
      ShadowMapAtlasCell implaced_cell(0, 0, it->x, it->y);
      auto result = squareIntersects(implaced_cell);
      const bool bIntersected = std::get<bool>(result);
      
      if (!bIntersected)
      {
         // can push
         if (!squareOutOfBounds(implaced_cell))
         {

         }
      }
   }

}
