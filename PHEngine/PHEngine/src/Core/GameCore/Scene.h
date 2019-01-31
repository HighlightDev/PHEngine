#pragma once
#include "Core/GameCore/Actor.h"
#include "Core/GameCore/FirstPersonCamera.h"
#include "Core/GraphicsCore/PrimitiveProxy/PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GraphicsCore/Renderer/DeferredRenderer.h"
#include "Core/GameCore/ShaderImplementation/ResolveTextureShader.h"

using namespace Graphics::Proxy;
using namespace Graphics::Renderer;
using namespace Game::ShaderImpl;

namespace Game
{

   class Scene
   {
      std::shared_ptr<ResolveTextureShader> m_resolveTexShader;

      DeferredRenderer m_deferredRenderer;

      size_t TotalSceneComponentIndex = 0;

      CameraBase* camera;

      glm::mat4 projectionMatrix;

      std::vector<Actor*> m_allActors;

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> m_sceneProxies;

   public:

      Scene();

      void Tick(float delta);

      void Render();

      void OnUpdatePrimitiveTransform_GameThread(size_t primitiveSceneProxyIndex, glm::mat4& newRelativeMatrix);

      template <typename PrimitiveType>
      void CreateAndAddComponent(ComponentData* componentData, Actor* addComponentToThisActor);

      void RemoveComponent(std::shared_ptr<Component> component);

      // TEMP
      void CameraRotate();

      ~Scene();
   };

}

