#pragma once

#include <memory>

#include "Core/GraphicsCore/Texture/ITexture.h"
#include "Core/GraphicsCore/Mesh/Skin.h"

using namespace Graphics::Mesh;
using namespace Graphics::Texture;

namespace Graphics
{

   namespace Data
   {
      struct WaterPlaneRenderData
      {
         std::shared_ptr<ITexture> m_waterDistortionMap;
         std::shared_ptr<ITexture> m_waterNormalMap;
         std::shared_ptr<Skin> m_skin;

         WaterPlaneRenderData(std::shared_ptr<ITexture> waterDistortionMap, std::shared_ptr<ITexture> waterNormalMap, std::shared_ptr<Skin> skin)
            : m_waterDistortionMap(waterDistortionMap)
            , m_waterNormalMap(waterNormalMap)
            , m_skin(skin)
         {
         }
         
      };
   }

}
