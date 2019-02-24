#include "Scene.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/UtilityCore/EngineMath.h"
#include "Core/GameCore/Components/SkyboxComponent.h"
#include "Core/GameCore/Components/ComponentCreatorFactory.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/GraphicsCore/Common/ScreenQuad.h"
#include "Core/GameCore/CameraBase.h"
#include "Core/GameCore/FirstPersonCamera.h"
#include "Core/GameCore/Components/DirectionalLightComponent.h"
#include "Core/GraphicsCore/Shadow/ShadowMapAtlasFactory.h"

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;
using namespace Graphics;

namespace Game
{

   Scene::Scene()
      : m_camera(new FirstPersonCamera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10)))
   {
      ShadowMapAtlas atlas;
      atlas.PushShadowMapSpace(glm::ivec2(1024, 1024));
      atlas.PushShadowMapSpace(glm::ivec2(512, 512));
      atlas.PushShadowMapSpace(glm::ivec2(256, 256));
      atlas.PushShadowMapSpace(glm::ivec2(256, 256));
      atlas.PushShadowMapSpace(glm::ivec2(2048, 2048));

      ShadowMapAtlasFactory::GetInstance()->ReserveShadowMapSpace(atlas);
      atlas.AllocateReservedMemory();
   }

   template <typename PrimitiveType>
   void Scene::CreateAndAddComponent_GameThread(ComponentData& componentData, Actor* addComponentToThisActor)
   {
      auto component = ComponentCreatorFactory<PrimitiveType>::CreateComponent(componentData);
      ComponentType type = component->GetComponentType();
      if ((type & ComponentType::SCENE_COMPONENT) == SCENE_COMPONENT)
      {
         SceneComponent* sceneComponentPtr = static_cast<SceneComponent*>(component.get());
         sceneComponentPtr->SetScene(this);
         if ((type & ComponentType::PRIMITIVE_COMPONENT) == PRIMITIVE_COMPONENT)
         {
            PrimitiveComponent* componentPtr = static_cast<PrimitiveComponent*>(sceneComponentPtr);
            componentPtr->PrimitiveProxyComponentId = PrimitiveComponent::TotalPrimitiveSceneProxyIndex++;
            auto sceneProxyShared = componentPtr->CreateSceneProxy();
            SceneProxies.push_back(sceneProxyShared);
         }
         else if ((type & ComponentType::LIGHT_COMPONENT) == LIGHT_COMPONENT)
         {
            LightComponent* componentPtr = static_cast<LightComponent*>(sceneComponentPtr);
            componentPtr->LightSceneProxyId = LightComponent::TotalLightSceneProxyId++;
            auto lightProxyShared = componentPtr->CreateSceneProxy();
            LightProxies.push_back(lightProxyShared);
         }
      }

      addComponentToThisActor->AddComponent(component);
   }

   void Scene::RemoveComponent_GameThread(std::shared_ptr<Component> component)
   {
      ComponentType type = component->GetComponentType();

      // Remove corresponding primitive proxy
      if ((type & ComponentType::PRIMITIVE_COMPONENT) == PRIMITIVE_COMPONENT)
      {
         PrimitiveComponent* componentPtr = static_cast<PrimitiveComponent*>(component.get());
         const size_t removeProxyIndex = componentPtr->PrimitiveProxyComponentId;
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
            PrimitiveComponent::TotalPrimitiveSceneProxyIndex--;
         }
      }
      if ((type & ComponentType::LIGHT_COMPONENT) == LIGHT_COMPONENT)
      {
         LightComponent* componentPtr = static_cast<LightComponent*>(component.get());
         const size_t removeProxyIndex = componentPtr->LightSceneProxyId;
         std::shared_ptr<LightSceneProxy> removeProxy = LightProxies.at(removeProxyIndex);

         // Remove proxy index offset
         for (auto& actor : AllActors)
         {
            actor->RemoveComponentIndexOffset(removeProxyIndex);
         }

         auto proxyIt = std::find(LightProxies.begin(), LightProxies.end(), removeProxy);
         if (proxyIt != LightProxies.end())
         {
            LightProxies.erase(proxyIt);
            LightComponent::TotalLightSceneProxyId--;
         }
      }

      Actor* ownerActor = component->GetOwner();
      if (ownerActor)
      {
         ownerActor->RemoveComponent(component);
      }
   }

   void Scene::OnUpdatePrimitiveComponentTransform_GameThread(size_t primitiveSceneProxyIndex, glm::mat4& newRelativeMatrix)
   {
      if (primitiveSceneProxyIndex < SceneProxies.size())
      {
         SceneProxies[primitiveSceneProxyIndex]->SetTransformationMatrix(newRelativeMatrix);
      }
   }

   void Scene::OnUpdateLightComponentTransform_GameThread(size_t lightSceneProxyIndex, glm::mat4& newRelativeMatrix)
   {
      if (lightSceneProxyIndex < LightProxies.size())
      {
         LightProxies[lightSceneProxyIndex]->SetTransformationMatrix(newRelativeMatrix);
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

   void Scene::CameraMove()
   {
      FirstPersonCamera* camera = static_cast<FirstPersonCamera*>(m_camera);
      if (camera)
      {
         camera->moveCamera(0);
      }
   }

   void Scene::AddTestActors()
   {
      const float aspectRatio = 16.0f / 9.0f;
      ProjectionMatrix = glm::perspective<float>(DEG_TO_RAD(60), aspectRatio, 1, 100);

      // DirectionalLight
      {
         DirectionalLightComponentData mData(glm::vec3(0), glm::vec3(0), glm::vec3(1),
            glm::vec3(1, 0, 0),
            glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.68f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f));
         Actor* dirLightActor = new Actor(new SceneComponent());
         CreateAndAddComponent_GameThread<DirectionalLightComponent>(mData, dirLightActor);
         AllActors.push_back(dirLightActor);
      }

      const auto& folderManager = Common::FolderManager::GetInstance();
      // HOUSE
      {
         StaticMeshComponentData mData(folderManager->GetModelPath() + "City_House_2_BI.obj", glm::vec3(), glm::vec3(), glm::vec3(1), folderManager->GetShadersPath() + "testVS.glsl",
            folderManager->GetShadersPath() + "testFS.glsl", folderManager->GetAlbedoTexturePath() + "city_house_2_Col.png", folderManager->GetNormalMapPath() + "city_house_2_Nor.png",
            folderManager->GetSpecularMapPath() + "city_house_2_Spec.png");
         Actor* houseActor = new Actor(new SceneComponent(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<StaticMeshComponent>(mData, houseActor);
         AllActors.push_back(houseActor);
      }

      // SKYBOX
      {
         float SKYBOX_SIZE = 50.0f;

         StringStreamWrapper::ToString(
            folderManager->GetCubemapTexturePath(), "Day/", "right.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "left.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "top.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "bottom.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "back.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "front.png");
         auto dTexPath = StringStreamWrapper::FlushString();

         SkyboxComponentData mData(SKYBOX_SIZE, 5.0f, std::move(folderManager->GetShadersPath() + "tSkyboxVS.glsl"),
            std::move(folderManager->GetShadersPath() + "tSkyboxFS.glsl"), std::move(dTexPath));
         Actor* skyboxActor = new Actor(new SceneComponent(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<SkyboxComponent>(mData, skyboxActor);
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
