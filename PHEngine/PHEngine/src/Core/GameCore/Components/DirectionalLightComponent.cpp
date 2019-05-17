#include "DirectionalLightComponent.h"

#include "Core/GraphicsCore/SceneProxy/DirectionalLightSceneProxy.h"
#include "Core/GameCore/Scene.h"
#include "Core/GraphicsCore/Shadow/ProjectedShadowInfo.h"

using namespace Graphics;

namespace Game
{

   DirectionalLightComponent::DirectionalLightComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, const DirectionalLightRenderData& renderData)
      : LightComponent(translation, rotation, scale)
      , m_renderData(renderData)
   {
      EventBase::GetInstance()->AddListener(this);
   }

   DirectionalLightComponent::~DirectionalLightComponent()
   {
      EventBase::GetInstance()->RemoveListener(this);
   }

   std::shared_ptr<LightSceneProxy> DirectionalLightComponent::CreateSceneProxy() const
   {
      return std::make_shared<DirectionalLightSceneProxy>(this);
   }

   void DirectionalLightComponent::UpdateRelativeMatrix(glm::mat4& parentRelativeMatrix)
   {
      Base::UpdateRelativeMatrix(parentRelativeMatrix);
   }

   void DirectionalLightComponent::Tick(float deltaTime)
   {
      Base::Tick(deltaTime);

      // SetRotationAxisY(m_rotation.y + 0.01f);
   }

   uint64_t DirectionalLightComponent::GetComponentType() const
   {
      return DIR_LIGHT_COMPONENT;
   }

   void DirectionalLightComponent::ProcessEvent(const EventBase::EventData_t& data)
   {
      m_scene->ExecuteOnRenderThread([=]()
      {
         ProjectedShadowInfo* shadowInfo = m_scene->LightProxies[LightSceneProxyId]->GetShadowInfo();
         if (shadowInfo)
         {
            shadowInfo->Offset = data;
         }
      });
   }

}
