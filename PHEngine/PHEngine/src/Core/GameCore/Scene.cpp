#include "Scene.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/UtilityCore/EngineMath.h"
#include "Core/GameCore/Components/SkyboxComponent.h"
#include "Core/GameCore/Components/ComponentCreatorFactory.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/GraphicsCore/Common/ScreenQuad.h"
#include "Core/GameCore/CameraBase.h"
#include "Core/GameCore/FirstPersonCamera.h"
#include "Core/GameCore/ThirdPersonCamera.h"
#include "Core/GameCore/Components/DirectionalLightComponent.h"
#include "Core/GraphicsCore/Shadow/ProjectedDirShadowInfo.h"
#include "Core/GraphicsCore/Shadow/ProjectedPointShadowInfo.h"
#include "Core/GameCore/Components/PointLightComponent.h"
#include "Core/GameCore/Components/BillboardComponent.h"
#include "Core/GameCore/Components/ComponentData/BillboardComponentData.h"
#include "Core/GameCore/Components/InputComponent.h"
#include "Core/GraphicsCore/TextureAtlas/TextureAtlasFactory.h"
#include "Core/GameCore/Components/ComponentData/InputComponentData.h"

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;
using namespace Graphics;

namespace Game
{

   Scene::Scene(InterThreadCommunicationMgr& interThreadMgr)
      : m_interThreadMgr(interThreadMgr)
      //, m_camera(new FirstPersonCamera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10)))
      , m_camera(new ThirdPersonCamera(glm::vec3(0.5f, -0.8f, 0), 20))
   {
      
   }

   void Scene::PostConstructorInitialize()
   {
      for (auto& lightProxy : LightProxies)
      {
         lightProxy->PostConstructorInitialize();
      }
   }

   template <typename PrimitiveType>
   void Scene::CreateAndAddComponent_GameThread(ComponentData& componentData, std::shared_ptr<Actor> owner)
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
         owner->AddMovementComponent(component);
      }
      else if ((type & INPUT_COMPONENT) == INPUT_COMPONENT)
      {
         owner->AddInputComponent(component);
      }
      else
      {
         owner->AddComponent(component);
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

   void Scene::ExecuteOnRenderThread(EnqueueJobPolicy policy, std::function<void(void)> gameThreadJob) const
   {
      ENQUEUE_RENDER_THREAD_JOB(m_interThreadMgr, policy, gameThreadJob);
   }

   void Scene::ExecuteOnGameThread(EnqueueJobPolicy policy, std::function<void(void)> renderThreadJob) const
   {
      ENQUEUE_GAME_THREAD_JOB(m_interThreadMgr, policy, renderThreadJob);
   }

   void Scene::OnUpdatePrimitiveComponentTransform_GameThread(size_t primitiveSceneProxyIndex, const glm::mat4& newRelativeMatrix)
   {
      if (primitiveSceneProxyIndex < SceneProxies.size())
      {
         ENQUEUE_RENDER_THREAD_JOB(m_interThreadMgr, EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH,
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
         ENQUEUE_RENDER_THREAD_JOB(m_interThreadMgr, EnqueueJobPolicy::IF_DUPLICATE_REPLACE_AND_PUSH,
            [=]()
         {
            LightProxies[lightSceneProxyIndex]->SetTransformationMatrix(newRelativeMatrix);
         });
      }
   }

   void Scene::Tick_GameThread(float delta)
   {
      m_camera->Tick(delta);
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
      if (m_camera->GetCameraType() == CameraBase::CameraType::FIRST_PERSON)
      {
         static_cast<FirstPersonCamera*>(m_camera)->MoveCamera(0);
      }
   }

   void Scene::AddTestActors()
   {
      const auto& folderManager = Common::FolderManager::GetInstance();

      const float aspectRatio = 16.0f / 9.0f;
      ProjectionMatrix = glm::perspective<float>(DEG_TO_RAD(60), aspectRatio, 1, 1000);
      #if 0
      // PointLight
      {
         // 0
         std::shared_ptr<Actor> pointLightActor = std::make_shared<Actor>(std::make_shared<SceneComponent>(glm::vec3(0, 50, 0), glm::vec3(), glm::vec3(1)));

         auto pointLightTextureAtlasRequest = TextureAtlasFactory::GetInstance()->AddTextureCubeAtlasRequest(glm::ivec2(512, 512));
         ProjectedPointShadowInfo* pointLightInfo = new ProjectedPointShadowInfo(pointLightTextureAtlasRequest);

         PointLightComponentData mData(glm::vec3(0), glm::vec3(), glm::vec3(0.005f, 0, 0), 3000, glm::vec3(0.2f, 0.2f, 0.2f), 
            glm::vec3(0.68f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), pointLightInfo);
         CreateAndAddComponent_GameThread<PointLightComponent>(mData, pointLightActor);

#if DEBUG

         std::shared_ptr<Actor> debugVisualLigthActor = std::make_shared<Actor>(std::make_shared<SceneComponent>(glm::vec3(0, 0, 0), glm::vec3(), glm::vec3(1)));

         BillboardComponentData mBillboardData(glm::vec3(0, 5, 0), glm::vec3(), glm::vec3(1),
            folderManager->GetShadersPath() + "lampVS.glsl", folderManager->GetShadersPath() + "lampFS.glsl", folderManager->GetShadersPath() + "lampGS.glsl",
            folderManager->GetEditorTexturePath() + "lamp.png");
         CreateAndAddComponent_GameThread<BillboardComponent>(mBillboardData, debugVisualLigthActor);

         CubemapComponentData mCubemapData(glm::vec3(), glm::vec3(), glm::vec3(1), folderManager->GetShadersPath() + "tSkyboxVS.glsl", folderManager->GetShadersPath() + "cubemapRendererFS.glsl", pointLightTextureAtlasRequest);
         CreateAndAddComponent_GameThread<CubemapComponent>(mCubemapData, debugVisualLigthActor);
         pointLightActor->AttachActor(debugVisualLigthActor);
#endif

         AllActors.push_back(pointLightActor);
      }
#endif

      // DirectionalLight
      {
         // 0
         auto directionalLightTextureAtlasRequest = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(512, 512));
         ProjectedDirShadowInfo* dirLightInfo = new ProjectedDirShadowInfo(directionalLightTextureAtlasRequest);

         DirectionalLightComponentData mData(glm::vec3(0),
            glm::vec3(1, 0, 0),
            glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.68f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), dirLightInfo);
         std::shared_ptr<Actor> dirLightActor = std::make_shared<Actor>(std::make_shared<SceneComponent>());
         CreateAndAddComponent_GameThread<DirectionalLightComponent>(mData, dirLightActor);
         AllActors.push_back(dirLightActor);

         // 1
         auto directionalLightTextureAtlasRequest1 = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(512, 512));
         ProjectedDirShadowInfo* dirLightInfo1 = new ProjectedDirShadowInfo(directionalLightTextureAtlasRequest1);

         DirectionalLightComponentData mData1(glm::vec3(0),
            glm::vec3(-1, 0, 0),
            glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.68f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), dirLightInfo1);
         std::shared_ptr<Actor> dirLightActor1 = std::make_shared<Actor>(std::make_shared<SceneComponent>());
         CreateAndAddComponent_GameThread<DirectionalLightComponent>(mData1, dirLightActor1);
         AllActors.push_back(dirLightActor1);

         // 2
         auto directionalLightTextureAtlasRequest2 = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(512, 512));
         ProjectedDirShadowInfo* dirLightInfo2 = new ProjectedDirShadowInfo(directionalLightTextureAtlasRequest2);

         DirectionalLightComponentData mData2(glm::vec3(0),
            glm::vec3(0, 0, 1),
            glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.68f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), dirLightInfo2);
         std::shared_ptr<Actor> dirLightActor2 = std::make_shared<Actor>(std::make_shared<SceneComponent>());
         CreateAndAddComponent_GameThread<DirectionalLightComponent>(mData2, dirLightActor2);
         AllActors.push_back(dirLightActor2);

         // 3
         auto directionalLightTextureAtlasRequest3 = TextureAtlasFactory::GetInstance()->AddTextureAtlasRequest(glm::ivec2(512, 512));
         ProjectedDirShadowInfo* dirLightInfo3 = new ProjectedDirShadowInfo(directionalLightTextureAtlasRequest3);

         DirectionalLightComponentData mData3(glm::vec3(0), glm::vec3(0, 0, -1),
            glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.68f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), dirLightInfo3);
         std::shared_ptr<Actor> dirLightActor3 = std::make_shared<Actor>(std::make_shared<SceneComponent>());
         CreateAndAddComponent_GameThread<DirectionalLightComponent>(mData3, dirLightActor3);
         AllActors.push_back(dirLightActor3);
      }

      // HOUSE
      {
         StaticMeshComponentData mData(folderManager->GetModelPath() + "City_House_2_BI.obj", glm::vec3(0), glm::vec3(), glm::vec3(5), folderManager->GetShadersPath() + "testVS.glsl",
            folderManager->GetShadersPath() + "testFS.glsl", folderManager->GetAlbedoTexturePath() + "city_house_2_Col.png", folderManager->GetNormalMapPath() + "city_house_2_Nor.png",
            folderManager->GetSpecularMapPath() + "city_house_2_Spec.png");
         std::shared_ptr<Actor> houseActor = std::make_shared<Actor>(std::make_shared<SceneComponent>(std::move(glm::vec3(10)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<StaticMeshComponent>(mData, houseActor);
         AllActors.push_back(houseActor);
      }

      // SKELETAL MESH
      {
         SkeletalMeshComponentData mData(folderManager->GetModelPath() + "model.dae", glm::vec3(0, 0 , 0), glm::vec3(270, 0, 0), glm::vec3(1), folderManager->GetShadersPath() + "skeletalMeshVS.glsl",
            folderManager->GetShadersPath() + "skeletalMeshFS.glsl", folderManager->GetAlbedoTexturePath() + "diffuse.png", folderManager->GetNormalMapPath() + "dummy_nm.png",
            folderManager->GetSpecularMapPath() + "city_house_2_Spec.png");
         std::shared_ptr<Actor> skeletActor = std::make_shared<Actor>(std::make_shared<SceneComponent>(std::move(glm::vec3(10)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<SkeletalMeshComponent>(mData, skeletActor);

         InputComponentData inputComponentData;
         CreateAndAddComponent_GameThread<InputComponent>(inputComponentData, skeletActor);
         MovementComponentData movementComponentData(glm::vec3(0), m_camera);
         CreateAndAddComponent_GameThread<MovementComponent>(movementComponentData, skeletActor);

         m_playerController.SetPlayerActor(skeletActor);

         AllActors.push_back(skeletActor);

         if (m_camera->GetCameraType() == CameraBase::CameraType::THIRD_PERSON)
         {
            static_cast<ThirdPersonCamera*>(m_camera)->SetThirdPersonTarget(skeletActor);
         }
      }

      // SKYBOX
      {
         StringStreamWrapper::ToString(
            folderManager->GetCubemapTexturePath(), "Day/", "right.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "left.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "top.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "bottom.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "back.png", ",",
            folderManager->GetCubemapTexturePath(), "Day/", "front.png");
         auto dTexPath = StringStreamWrapper::FlushString();

         SkyboxComponentData mData(glm::vec3(140.0f), 5.0f, std::move(folderManager->GetShadersPath() + "tSkyboxVS.glsl"),
            std::move(folderManager->GetShadersPath() + "tSkyboxFS.glsl"), std::move(dTexPath));
         std::shared_ptr<Actor> skyboxActor = std::make_shared<Actor>(std::make_shared<SceneComponent>(std::move(glm::vec3(0)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent_GameThread<SkyboxComponent>(mData, skyboxActor);
         AllActors.push_back(skyboxActor);
      }

      TextureAtlasFactory::GetInstance()->AllocateAtlasSpace();
   }

   Scene::~Scene()
   {
      AllActors.clear();
   }

}
