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
#include "Core/GameCore/Components/InputComponent.h"
#include "Core/GraphicsCore/Shadow/TextureAtlasFactory.h"
#include "Core/GameCore/Components/ComponentData/InputComponentData.h"

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;
using namespace Graphics;

namespace Game
{

   Scene::Scene(InterThreadCommunicationMgr& interThreadMgr)
      : m_interThreadMgr(interThreadMgr)
      , m_camera(new FirstPersonCamera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10)))
   {
      
   }

   template <typename PrimitiveType>
   void Scene::CreateAndAddComponent_GameThread(ComponentData& componentData, Actor* addComponentToThisActor)
   {
      auto component = ComponentCreatorFactory<PrimitiveType>::CreateComponent(componentData);
      uint64_t type = component->GetComponentType();
      if ((type & SCENE_COMPONENT) == SCENE_COMPONENT)
      {
         SceneComponent* sceneComponentPtr = static_cast<SceneComponent*>(component.get());
         sceneComponentPtr->SetScene(this);
         if ((type & PRIMITIVE_COMPONENT) == PRIMITIVE_COMPONENT)
         {
            PrimitiveComponent* componentPtr = static_cast<PrimitiveComponent*>(sceneComponentPtr);
            componentPtr->PrimitiveProxyComponentId = PrimitiveComponent::TotalPrimitiveSceneProxyIndex++;
            auto sceneProxyShared = componentPtr->CreateSceneProxy();
            SceneProxies.push_back(sceneProxyShared);
         }
         else if ((type & LIGHT_COMPONENT) == LIGHT_COMPONENT)
         {
            LightComponent* componentPtr = static_cast<LightComponent*>(sceneComponentPtr);
            componentPtr->LightSceneProxyId = LightComponent::TotalLightSceneProxyId++;
            auto lightProxyShared = componentPtr->CreateSceneProxy();
            LightProxies.push_back(lightProxyShared);
         }
      }

      if ((type & MOVEMENT_COMPONENT) == MOVEMENT_COMPONENT)
      {
         addComponentToThisActor->AddMovementComponent(std::dynamic_pointer_cast<MovementComponent>(component));
      }
      else if ((type & INPUT_COMPONENT) == INPUT_COMPONENT)
      {
         std::shared_ptr<InputComponent> inputComp = std::dynamic_pointer_cast<InputComponent>(component);
         inputComp->SetOwner(addComponentToThisActor);
         addComponentToThisActor->AddInputComponent(inputComp);
      }
      else
      {
         addComponentToThisActor->AddComponent(component);
      }
   }

   void Scene::RemoveComponent_GameThread(std::shared_ptr<Component> component)
   {
       uint64_t type = component->GetComponentType();

      // Remove corresponding primitive proxy
      if ((type & PRIMITIVE_COMPONENT) == PRIMITIVE_COMPONENT)
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
      if ((type & LIGHT_COMPONENT) == LIGHT_COMPONENT)
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
         if ((type & MOVEMENT_COMPONENT) == MOVEMENT_COMPONENT)
         {
            ownerActor->RemoveMovementComponent();
         }
         else if ((type & INPUT_COMPONENT) == INPUT_COMPONENT)
         {
            ownerActor->RemoveInputComponent();
         }
         else
         {
            ownerActor->RemoveComponent(component);
         }
      }
   }

   void Scene::ExecuteOnRenderThread(std::function<void(void)> gameThreadJob) const
   {
      ENQUEUE_RENDER_THREAD_JOB(m_interThreadMgr, gameThreadJob);
   }

   void Scene::ExecuteOnGameThread(std::function<void(void)> renderThreadJob) const
   {
      ENQUEUE_GAME_THREAD_JOB(m_interThreadMgr, renderThreadJob);
   }

   void Scene::OnUpdatePrimitiveComponentTransform_GameThread(size_t primitiveSceneProxyIndex, const glm::mat4& newRelativeMatrix)
   {
      if (primitiveSceneProxyIndex < SceneProxies.size())
      {
         ENQUEUE_RENDER_THREAD_JOB(m_interThreadMgr,
            [=]()
         {
            SceneProxies[primitiveSceneProxyIndex]->SetTransformationMatrix(newRelativeMatrix);
         });
      }
   }

   void Scene::OnUpdateLightComponentTransform_GameThread(size_t lightSceneProxyIndex, const glm::mat4& newRelativeMatrix)
   {
      if (lightSceneProxyIndex < LightProxies.size())
      {
         ENQUEUE_RENDER_THREAD_JOB(m_interThreadMgr,
            [=]()
         {
            LightProxies[lightSceneProxyIndex]->SetTransformationMatrix(newRelativeMatrix);
         });
      }
   }

   void Scene::Tick_GameThread(float delta)
   {
      for (auto& actor : AllActors)
      {
         actor->Tick(delta);
      }
      m_playerController.Tick(delta);
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

      // TEMPORARY
      auto req1 = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(1000, 1000));
      auto req2 = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(500, 500));
      auto req3 = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(256, 256));
      auto req4 = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(2000, 2000));
      TextureAtlasFactory::GetInstance()->ReserveShadowMapSpace();

      // DirectionalLight
      {
         
         ProjectedShadowInfo* dirLightInfo = nullptr;

         DirectionalLightComponentData mData(glm::vec3(0), glm::vec3(0), glm::vec3(1),
            glm::vec3(1, 0, 0),
            glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.68f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), dirLightInfo);
         Actor* dirLightActor = new Actor(new SceneComponent());
         CreateAndAddComponent_GameThread<DirectionalLightComponent>(mData, dirLightActor);
         AllActors.push_back(dirLightActor);
      }

      const auto& folderManager = Common::FolderManager::GetInstance();
      // HOUSE
      {
         StaticMeshComponentData mData(folderManager->GetModelPath() + "City_House_2_BI.obj", glm::vec3(10, 10, 10), glm::vec3(), glm::vec3(1), folderManager->GetShadersPath() + "testVS.glsl",
            folderManager->GetShadersPath() + "testFS.glsl", folderManager->GetAlbedoTexturePath() + "city_house_2_Col.png", folderManager->GetNormalMapPath() + "city_house_2_Nor.png",
            folderManager->GetSpecularMapPath() + "city_house_2_Spec.png");
         Actor* houseActor = new Actor(new SceneComponent(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<StaticMeshComponent>(mData, houseActor);

         //InputComponentData inputComponentData;
         //CreateAndAddComponent_GameThread<InputComponent>(inputComponentData, houseActor);
         MovementComponentData movementComponentData(glm::vec3(0), m_camera);
         CreateAndAddComponent_GameThread<MovementComponent>(movementComponentData, houseActor);

         m_playerController.SetPlayerActor(houseActor);
         AllActors.push_back(houseActor);
      }

      // SKELETAL MESH
      {
         SkeletalMeshComponentData mData(folderManager->GetModelPath() + "model.dae", glm::vec3(), glm::vec3(270, 0, 0), glm::vec3(1), folderManager->GetShadersPath() + "skeletalMeshVS.glsl",
            folderManager->GetShadersPath() + "skeletalMeshFS.glsl", folderManager->GetAlbedoTexturePath() + "diffuse.png", folderManager->GetNormalMapPath() + "city_house_2_Nor.png",
            folderManager->GetSpecularMapPath() + "city_house_2_Spec.png");
         Actor* skeletActor = new Actor(new SceneComponent(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<SkeletalMeshComponent>(mData, skeletActor);

         AllActors.push_back(skeletActor);
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
