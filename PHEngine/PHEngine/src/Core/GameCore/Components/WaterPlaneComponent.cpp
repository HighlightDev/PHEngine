#include "WaterPlaneComponent.h"
#include "Core/GameCore/Scene.h"
#include "Core/GraphicsCore/SceneProxy/WaterPlaneSceneProxy.h"

#include <glm/vec3.hpp>

namespace Game
{

   WaterPlaneComponent::WaterPlaneComponent(WaterQualityFlag waterQuality)
      : PrimitiveComponent(glm::vec3(), glm::vec3(), glm::vec3())
      , m_renderData(nullptr, nullptr, nullptr, nullptr)
      , m_waterQuality(waterQuality)
   {
   }

   WaterPlaneComponent::~WaterPlaneComponent()
   {
   }
   
   uint64_t WaterPlaneComponent::GetComponentType() const
   {
      return WATER_PLANE_COMPONENT;
   }

   void WaterPlaneComponent::Tick(float deltaTime)
   {
      Base::Tick(deltaTime);

      m_moveFactor += m_waveSpeed * deltaTime;
      m_moveFactor = std::truncf(m_moveFactor);

      m_scene->ExecuteOnRenderThread(EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH, GetObjectId(), [=]() {

         WaterPlaneSceneProxy* proxyPtr = static_cast<WaterPlaneSceneProxy*>(m_scene->SceneProxies[PrimitiveProxyComponentId].get());
         proxyPtr->SetMoveFactor(m_moveFactor);
      });
   }

   float WaterPlaneComponent::GetMoveFactor() const
   {
      return m_moveFactor;
   }

   float WaterPlaneComponent::GetWaveStrength() const
   {
      return m_waveStrength;
   }

   float WaterPlaneComponent::GetTransparencyDepth() const {

      return m_transparencyDepth;
   }

   void WaterPlaneComponent::SetWaveStrength(float waveStr)
   {
      m_waveStrength = waveStr;

      m_scene->ExecuteOnRenderThread(EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH, GetObjectId(), [=]() {

         WaterPlaneSceneProxy* proxyPtr = static_cast<WaterPlaneSceneProxy*>(m_scene->SceneProxies[PrimitiveProxyComponentId].get());
         proxyPtr->SetWaveStrength(m_waveStrength);
      });
   }

   void WaterPlaneComponent::SetTransparencyDepth(float transparencyDepth)
   {
      m_transparencyDepth = transparencyDepth;

      m_scene->ExecuteOnRenderThread(EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH, GetObjectId(), [=]() {

         WaterPlaneSceneProxy* proxyPtr = static_cast<WaterPlaneSceneProxy*>(m_scene->SceneProxies[PrimitiveProxyComponentId].get());
         proxyPtr->SetTransparencyDepth(m_transparencyDepth);
      });
   }

   std::shared_ptr<PrimitiveSceneProxy> WaterPlaneComponent::CreateSceneProxy() const
   {
      return std::make_shared<WaterPlaneSceneProxy>(this);
   }

   float WaterPlaneComponent::GetNearClipPlane() const 
   {
      return m_nearClipPlane;
   }

   float WaterPlaneComponent::GetFarClipPlane() const
   {
      return m_farClipPlane;
   }

   void WaterPlaneComponent::SetNearClipPlane(float nearClipPlane)
   {
      m_nearClipPlane = nearClipPlane;
      m_scene->ExecuteOnRenderThread(EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH, GetObjectId(), [=]() {

         WaterPlaneSceneProxy* proxyPtr = static_cast<WaterPlaneSceneProxy*>(m_scene->SceneProxies[PrimitiveProxyComponentId].get());
         proxyPtr->SetNearClipPlane(m_nearClipPlane);
      });
   }

   void WaterPlaneComponent::SetFarClipPlane(float farClipPlane)
   {
      m_farClipPlane = farClipPlane;
      m_scene->ExecuteOnRenderThread(EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH, GetObjectId(), [=]() {

         WaterPlaneSceneProxy* proxyPtr = static_cast<WaterPlaneSceneProxy*>(m_scene->SceneProxies[PrimitiveProxyComponentId].get());
         proxyPtr->SetFarClipPlane(m_farClipPlane);
      });
   }

}
