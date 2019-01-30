#include "Scene.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/UtilityCore/EngineMath.h"
#include "Core/GameCore/Components/SkyboxComponent.h"
#include "Core/GameCore/Components/ComponentCreatorFactory.h"
#include "Core/CommonCore/FolderManager.h"

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;

namespace Game
{

   Scene::Scene()
      : camera(new FirstPersonCamera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10)))
   {
      const float aspectRatio = 16.0f / 9.0f;
      projectionMatrix = glm::perspective<float>(DEG_TO_RAD(60), aspectRatio, 1, 100);

      // HOUSE
      {
         StaticMeshComponentData mData(FolderManager::GetInstance()->GetModelPath() + "City_House_2_BI.obj", glm::vec3(), glm::vec3(), glm::vec3(1), Common::FolderManager::GetInstance()->GetShadersPath() + "testVS.glsl",
            Common::FolderManager::GetInstance()->GetShadersPath() + "testFS.glsl", FolderManager::GetInstance()->GetAlbedoTexturePath() + "city_house_2_Col.png");
         Actor houseActor = Actor(new SceneComponent(std::move(glm::vec3(10)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         AddPrimitiveComponent<StaticMeshComponent>(&mData, houseActor);
         m_allActors.emplace_back(std::move(houseActor));
      }

      // SKYBOX
      {
         float SKYBOX_SIZE = 50.0f;

         StringStreamWrapper::ToString(
            FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "right.png", ",",
            FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "left.png", ",",
            FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "top.png", ",",
            FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "bottom.png", ",",
            FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "back.png", ",",
            FolderManager::GetInstance()->GetCubemapTexturePath(), "Day/", "front.png");
         auto dTexPath = StringStreamWrapper::FlushString();

         SkyboxComponentData mData(SKYBOX_SIZE, 5.0f, std::move(FolderManager::GetInstance()->GetShadersPath() + "tSkyboxVS.glsl"),
            std::move(FolderManager::GetInstance()->GetShadersPath() + "tSkyboxFS.glsl"), std::move(dTexPath));
         Actor skyboxActor = Actor(new SceneComponent(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddPrimitiveComponent<SkyboxComponent>(&mData, skyboxActor);
         m_allActors.emplace_back(std::move(skyboxActor));
      }
   }

   template <typename PrimitiveType>
   void Scene::CreateAndAddPrimitiveComponent(ComponentData* primitiveComponentData, Actor& addComponentToThisActor)
   {
      auto primitiveComponent = ComponentCreatorFactory<PrimitiveType>::CreateComponent(*primitiveComponentData);
      PrimitiveComponent* componentPtr = static_cast<PrimitiveComponent*>(primitiveComponent.get());
      auto sceneProxyUnique = std::move(componentPtr->CreateSceneProxy());
      m_proxies.push_back(sceneProxyUnique);
      addComponentToThisActor.AddComponent(primitiveComponent);
   }

   void Scene::Render()
   {
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(0, 0, 0, 0);

      glm::mat4 viewMatrix = camera->GetViewMatrix();

      for (auto& proxy : m_proxies)
      {
         proxy->Render(viewMatrix, projectionMatrix);
      }
   }

   void Scene::Tick(float delta)
   {
      for (auto& actor : m_allActors)
      {
         actor.Tick(0.05f);
      }
   }

   void Scene::CameraRotate()
   {
      camera->Rotate();
   }

   Scene::~Scene()
   {
   }

}
