#include "Scene.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/UtilityCore/EngineMath.h"
#include "Core/GameCore/Components/SkyboxComponent.h"
#include "Core/GameCore/Components/ComponentCreatorFactory.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/GraphicsCore/Common/ScreenQuad.h"
#include "Core/GameCore/CameraBase.h"
#include "Core/GameCore/FirstPersonCamera.h"

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;
using namespace Graphics;

namespace Game
{

   Scene::Scene()
      : m_camera(new FirstPersonCamera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10)))
   {
   }

   template <typename PrimitiveType>
   void Scene::CreateAndAddComponent_GameThread(ComponentData* componentData, Actor* addComponentToThisActor)
   {
      auto component = ComponentCreatorFactory<PrimitiveType>::CreateComponent(*componentData);
      ComponentType type = component->GetComponentType();
      if (type & ComponentType::SCENE_COMPONENT)
      {
         SceneComponent* sceneComponentPtr = static_cast<SceneComponent*>(component.get());
         sceneComponentPtr->SetScene(this);
         if (type & ComponentType::PRIMITIVE_COMPONENT)
         {
            PrimitiveComponent* componentPtr = static_cast<PrimitiveComponent*>(sceneComponentPtr);
            componentPtr->SceneProxyComponentId = PrimitiveComponent::TotalSceneComponentIndex++;
            auto sceneProxyUnique = std::move(componentPtr->CreateSceneProxy());
            SceneProxies.push_back(sceneProxyUnique);
         }
      }

      addComponentToThisActor->AddComponent(component);
   }

   void Scene::RemoveComponent_GameThread(std::shared_ptr<Component> component)
   {
      ComponentType type = component->GetComponentType();

      // Remove corresponding primitive proxy
      if (type & ComponentType::PRIMITIVE_COMPONENT)
      {
         PrimitiveComponent* componentPtr = static_cast<PrimitiveComponent*>(component.get());
         const size_t removeProxyIndex = componentPtr->SceneProxyComponentId;
         std::shared_ptr<PrimitiveSceneProxy> removeProxy = SceneProxies.at(removeProxyIndex);
      
         // Remove proxy index offset
         for (auto& actor : AllActors)
         {
            actor->RemoveComponentIndexOffset(removeProxyIndex);
         }

         auto proxyIt = std::find(SceneProxies.begin(), SceneProxies.end(), removeProxy);
         if (proxyIt != SceneProxies.end())
         {
            SceneProxies.erase(proxyIt);
            PrimitiveComponent::TotalSceneComponentIndex--;
         }
      }

      Actor* ownerActor = component->GetOwner();
      if (ownerActor)
      {
         ownerActor->RemoveComponent(component);
      }
   }

   void Scene::OnUpdatePrimitiveTransform_GameThread(size_t primitiveSceneProxyIndex, glm::mat4& newRelativeMatrix)
   {
      if (primitiveSceneProxyIndex < SceneProxies.size())
      {
         SceneProxies[primitiveSceneProxyIndex]->SetTransformationMatrix(newRelativeMatrix);
      }
   }

   void Scene::Tick_GameThread(float delta)
   {
      for (auto& actor : AllActors)
      {
         actor->Tick(0.05f);
      }
   }

   void Scene::CameraRotate()
   {
      m_camera->Rotate();
   }

   void Scene::AddTestActors()
   {
      const float aspectRatio = 16.0f / 9.0f;
      ProjectionMatrix = glm::perspective<float>(DEG_TO_RAD(60), aspectRatio, 1, 100);

      // HOUSE
      {
         StaticMeshComponentData mData(FolderManager::GetInstance()->GetModelPath() + "City_House_2_BI.obj", glm::vec3(), glm::vec3(), glm::vec3(1), Common::FolderManager::GetInstance()->GetShadersPath() + "testVS.glsl",
            Common::FolderManager::GetInstance()->GetShadersPath() + "testFS.glsl", FolderManager::GetInstance()->GetAlbedoTexturePath() + "city_house_2_Col.png");
         Actor* houseActor = new Actor(new SceneComponent(std::move(glm::vec3(10)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<StaticMeshComponent>(&mData, houseActor);
         AllActors.push_back(houseActor);
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
         Actor* skyboxActor = new Actor(new SceneComponent(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<SkyboxComponent>(&mData, skyboxActor);
         AllActors.push_back(skyboxActor);
      }
   }

   Scene::~Scene()
   {
      for (Actor* actor : AllActors)
      {
         delete actor;
      }
      AllActors.clear();
   }

}
