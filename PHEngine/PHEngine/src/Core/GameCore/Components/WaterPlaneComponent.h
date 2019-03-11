#pragma once
#include "PrimitiveComponent.h"
#include "Core/GraphicsCore/RenderData/WaterPlaneRenderData.h"

using namespace Graphics::Data;

namespace Game
{

   enum WaterQualityFlag
   {
      REFLECT_STATIC_MESH = 0x000001,
      REFLECT_SKELETAL_MESH = 0x000010,
      REFLECT_PARTICLES = 0x000100,
      REFRACT_STATIC_MESH = 0x001000,
      REFRACT_SKELETAL_MESH = 0x010000,
      REFRACT_PARTICLES = 0x100000
   };

   class WaterPlaneComponent :
      public PrimitiveComponent
   {

      float m_transparencyDepth;
      float m_waveSpeed;
      float m_waveStrength;
      float m_moveFactor;
      WaterQualityFlag m_waterQuality;
      WaterPlaneRenderData m_renderData;

   public:

      WaterPlaneComponent(WaterQualityFlag waterQuality = (WaterQualityFlag)(REFLECT_SKELETAL_MESH | REFRACT_STATIC_MESH | REFLECT_STATIC_MESH | REFRACT_SKELETAL_MESH));

      virtual ~WaterPlaneComponent();

      virtual ComponentType GetComponentType() const override;
      
      inline WaterPlaneRenderData GetRenderData() const {

         return m_renderData;
      }
   };

}

