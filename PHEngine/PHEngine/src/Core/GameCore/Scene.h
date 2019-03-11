#pragma once

#include "Core/GameCore/Actor.h"
#include "Core/GraphicsCore/SceneProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/LightComponent.h"
#include "Core/GameCore/Components/Component.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/InterThreadCommunicationMgr.h"

using namespace Graphics::Proxy;
using namespace Thread;

namespace Game
{

   class Scene
   {
   public:

      std::vector<Actor*> AllActors;

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> SceneProxies;

      std::vector<std::shared_ptr<LightSceneProxy>> LightProxies;

      glm::mat4 ProjectionMatrix;

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> ShadowGroupPrimitives;

   private:

      InterThreadCommunicationMgr& m_interThreadMgr;

      class CameraBase* m_camera;

   public:

      Scene(InterThreadCommunicationMgr& interThreadMgr);

      inline class CameraBase* GetCamera()
      {
         return m_camera;
      }

      void Tick_GameThread(float delta);

      void OnUpdatePrimitiveComponentTransform_GameThread(size_t primitiveSceneProxyIndex, glm::mat4& newRelativeMatrix);

      void OnUpdateLightComponentTransform_GameThread(size_t lightSceneProxyIndex, glm::mat4& newRelativeMatrix);

      template <typename PrimitiveType>
      void CreateAndAddComponent_GameThread(ComponentData& componentData, Actor* addComponentToThisActor);

      void RemoveComponent_GameThread(std::shared_ptr<Component> component);

      void ExecuteOnRenderThread(std::function<void(void)> gameThreadJob) const;

      void ExecuteOnGameThread(std::function<void(void)> renderThreadJob) const;

      // TEMP
      void CameraRotate();

      void CameraMove();

      void AddTestActors();

      ~Scene();
   };

}

