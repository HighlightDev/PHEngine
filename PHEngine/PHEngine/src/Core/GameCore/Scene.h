#pragma once

#include "Core/GameCore/Actor.h"
#include "Core/GraphicsCore/PrimitiveProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/Component.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GraphicsCore/Light/DirectionalLight.h"

using namespace Graphics::Proxy;
using namespace Graphics::Light;

namespace Game
{

   class Scene
   {
   public:

      std::vector<Actor*> AllActors;

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> SceneProxies;

      glm::mat4 ProjectionMatrix;

      std::vector<DirectionalLight> m_dirLightSources;

   private:

      class CameraBase* m_camera;

   public:

      Scene();

      inline class CameraBase* GetCamera()
      {
         return m_camera;
      }

      void Tick_GameThread(float delta);

      void OnUpdatePrimitiveTransform_GameThread(size_t primitiveSceneProxyIndex, glm::mat4& newRelativeMatrix);

      template <typename PrimitiveType>
      void CreateAndAddComponent_GameThread(ComponentData& componentData, Actor* addComponentToThisActor);

      void RemoveComponent_GameThread(std::shared_ptr<Component> component);

      // TEMP
      void CameraRotate();

      void AddTestActors();

      ~Scene();
   };

}

