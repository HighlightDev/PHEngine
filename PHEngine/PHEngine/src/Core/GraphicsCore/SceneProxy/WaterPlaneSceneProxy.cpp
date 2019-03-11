#include "WaterPlaneSceneProxy.h"

namespace Graphics
{
   namespace Proxy
   {

      WaterPlaneSceneProxy::WaterPlaneSceneProxy(const WaterPlaneComponent* component)
         : PrimitiveSceneProxy(component->GetRelativeMatrix(), component->GetRenderData().m_skin, std::shared_ptr<ITexture>())
         , m_waterDistortionMap(component->GetRenderData().m_waterDistortionMap)
         , m_waterNormalMap(component->GetRenderData().m_waterNormalMap)
         , m_skin(component->GetRenderData().m_skin)
      {
         Init();
      }

      WaterPlaneSceneProxy::~WaterPlaneSceneProxy()
      {
      }

      void WaterPlaneSceneProxy::Init()
      {
         m_waterPlaneFramebuffer = std::make_unique<WaterPlaneFramebuffer>();
      }

   }
}
