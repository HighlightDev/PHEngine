#pragma once

#include "LightComponent.h"
#include "Core/GraphicsCore/SceneProxy/LightSceneProxy.h"
#include "Core/GraphicsCore/RenderData/DirectionalLightRenderData.h"

using namespace Graphics::Proxy;
using namespace Graphics::Data;

namespace Game
{

   class DirectionalLightComponent :
      public LightComponent
   {

      using Base = LightComponent;

   protected:

      DirectionalLightRenderData m_renderData;

   public:

      DirectionalLightComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, const DirectionalLightRenderData& renderData);

      virtual ~DirectionalLightComponent();

      virtual uint64_t GetComponentType() const override;

      virtual std::shared_ptr<LightSceneProxy> CreateSceneProxy() const override;

      inline const DirectionalLightRenderData& GetRenderData() const {

         return m_renderData;
      }

      virtual void UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix) override;
   };

}

