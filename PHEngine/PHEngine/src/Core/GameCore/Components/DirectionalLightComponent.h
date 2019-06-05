#pragma once

#include "LightComponent.h"
#include "Core/GraphicsCore/SceneProxy/LightSceneProxy.h"
#include "Core/GraphicsCore/RenderData/DirectionalLightRenderData.h"
#include "Core/GameCore/Event/CameraMovedEvent.h"

using namespace Graphics::Proxy;
using namespace Graphics::Data;
using namespace Event;

namespace Game
{

   class DirectionalLightComponent :
      public LightComponent, public CameraMovedEvent
   {

      using Base = LightComponent;
      using EventBase = CameraMovedEvent;

   protected:

      DirectionalLightRenderData m_renderData;

   public:

      DirectionalLightComponent(glm::vec3 rotation, const DirectionalLightRenderData& renderData);

      virtual ~DirectionalLightComponent();

      virtual uint64_t GetComponentType() const override;

      virtual std::shared_ptr<LightSceneProxy> CreateSceneProxy() const override;

      inline const DirectionalLightRenderData& GetRenderData() const {

         return m_renderData;
      }

      virtual void UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix) override;

      virtual void Tick(float deltaTime) override;

   protected:

      virtual void ProcessEvent(const EventBase::EventData_t& data) override;
   };

}

