#pragma once
#include "PrimitiveComponent.h"
#include "Core/GraphicsCore/RenderData/WaterPlaneRenderData.h"

using namespace Graphics::Data;

namespace Game
{

   class WaterPlaneComponent :
      public PrimitiveComponent
   {

      WaterPlaneRenderData m_renderData;

   public:
      WaterPlaneComponent();
      virtual ~WaterPlaneComponent();
   };

}

