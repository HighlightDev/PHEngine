#pragma once
#include "Core/GameCore/Actor.h"
#include "Core/GameCore/FirstPersonCamera.h"
#include "Core/GraphicsCore/PrimitiveProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"

using namespace Graphics::Proxy;

namespace Game
{

   class Scene
   {
      CameraBase* camera;

      glm::mat4 projectionMatrix;

      std::vector<Actor> m_allActors;

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> m_proxies;

   public:

      Scene();

      void Tick(float delta);

      void Render();

      template <typename PrimitiveType>
      void CreateAndAddPrimitiveComponent(ComponentData* primitiveComponentData, Actor& addComponentToThisActor);

      // TEMP
      void CameraRotate();

      ~Scene();
   };

}

