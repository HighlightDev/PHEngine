#pragma once

#include "Core/GameCore/Actor.h"
#include "Core/GraphicsCore/SceneProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/LightComponent.h"
#include "Core/GameCore/Components/Component.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GameCore/PlayerController.h"
#include "Core/InterThreadCommunicationMgr.h"

using namespace Graphics::Proxy;
using namespace Thread;

namespace Game
{

   class Scene
   {
   public:

      /*
      
      Physics environment 

      */

      //btDiscreteDynamicsWorld* m_dynamicsWorld;

      std::vector<std::shared_ptr<Actor>> AllActors;

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> SceneProxies;

      std::vector<std::shared_ptr<LightSceneProxy>> LightProxies;

      glm::mat4 ProjectionMatrix;

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> ShadowGroupPrimitives;

      PlayerController m_playerController;

   private:

      InterThreadCommunicationMgr& m_interThreadMgr;

      class ICamera* m_camera;

   public:

      Scene(InterThreadCommunicationMgr& interThreadMgr);

      void PostConstructorInitialize();

      inline class ICamera* GetCamera() const
      {
         return m_camera;
      }

      void Tick_GameThread(float delta);

      void OnUpdatePrimitiveComponentTransform_GameThread(size_t primitiveSceneProxyIndex, const uint64_t creatorObjectId, const uint64_t functionId, const glm::mat4& newRelativeMatrix);

      void OnUpdateLightComponentTransform_GameThread(size_t lightSceneProxyIndex, const uint64_t creatorObjectId, const uint64_t functionId, const glm::mat4& newRelativeMatrix);

      template <typename PrimitiveType>
      void CreateAndAddComponent_GameThread(ComponentData& componentData, std::shared_ptr<Actor> owner);

      void RemoveComponent_GameThread(std::shared_ptr<Component> component);

      void ExecuteOnRenderThread(EnqueueJobPolicy policy, const uint64_t creatorObjectId, const uint64_t functionId, const std::function<void(void)>& gameThreadJobCallback) const;

      void ExecuteOnGameThread(EnqueueJobPolicy policy, const uint64_t creatorObjectId, const uint64_t functionId, const std::function<void(void)>& renderThreadJobCallback) const;

      // TEMP
      void CameraRotate();

      void CameraMove();

      void AddTestActors();

      ~Scene();
   };

}

