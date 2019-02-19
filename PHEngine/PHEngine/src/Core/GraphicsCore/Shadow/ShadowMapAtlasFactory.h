#pragma once

#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <vector>
#include <glm/vec2.hpp>

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
};

struct ShadowMapAtlas
{
   std::vector<ShadowMapAtlasCell> Cells;

   /* Flag is true when memory for shadowmap was already allocated,
      to reuse space, you should first deallocate possessed memory space */
   bool bInvalidated = false;

   void AllocateReservedMemory();

   void DeallocateMemory();
};

class ShadowMapAtlasFactory
{
   static std::unique_ptr<ShadowMapAtlasFactory> m_instance;

   std::unordered_map<ShadowMapAtlas, std::vector<glm::ivec2>> m_reservations;

public:

   ShadowMapAtlasFactory();

   ~ShadowMapAtlasFactory();

   static std::unique_ptr<ShadowMapAtlasFactory>& GetInstance()
   {
      if (!m_instance)
         m_instance = std::make_unique<ShadowMapAtlasFactory>();

      return m_instance;
   }

   void PushShadowMapSpace(glm::ivec2 size, ShadowMapAtlas& atlas);

   void ReserveShadowMapSpace(ShadowMapAtlas& atlas);
};


