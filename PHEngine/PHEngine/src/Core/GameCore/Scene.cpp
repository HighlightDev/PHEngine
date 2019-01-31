#include "Scene.h"
#include "Core/UtilityCore/StringStreamWrapper.h"
#include "Core/UtilityCore/EngineMath.h"
#include "Core/GameCore/Components/SkyboxComponent.h"
#include "Core/GameCore/Components/ComponentCreatorFactory.h"
#include "Core/CommonCore/FolderManager.h"
#include "Core/GraphicsCore/Common/ScreenQuad.h"

using namespace Graphics::Texture;
using namespace Common;
using namespace EngineUtility;
using namespace Graphics;

namespace Game
{

   Scene::Scene()
      : m_deferredRenderer(GlobalProperties::GetInstance()->GetInputData().GetWindowWidth(), GlobalProperties::GetInstance()->GetInputData().GetWindowHeight())
      , camera(new FirstPersonCamera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10)))
   {
      std::string resolveTexShaderPath = Common::FolderManager::GetInstance()->GetShadersPath() + "resolveTextureVS.glsl" + "," + Common::FolderManager::GetInstance()->GetShadersPath() + "resolveTextureFS.glsl";
      m_resolveTexShader = std::dynamic_pointer_cast<ResolveTextureShader>(ShaderPool::GetInstance()->GetOrAllocateResource<ResolveTextureShader>(resolveTexShaderPath));

      const float aspectRatio = 16.0f / 9.0f;
      projectionMatrix = glm::perspective<float>(DEG_TO_RAD(60), aspectRatio, 1, 100);

      // HOUSE
      {
         StaticMeshComponentData mData(FolderManager::GetInstance()->GetModelPath() + "City_House_2_BI.obj", glm::vec3(), glm::vec3(), glm::vec3(1), Common::FolderManager::GetInstance()->GetShadersPath() + "testVS.glsl",
            Common::FolderManager::GetInstance()->GetShadersPath() + "testFS.glsl", FolderManager::GetInstance()->GetAlbedoTexturePath() + "city_house_2_Col.png");
         Actor* houseActor = new Actor(new SceneComponent(std::move(glm::vec3(10)), std::move(glm::vec3(0)), std::move(glm::vec3(1))));
         CreateAndAddComponent<StaticMeshComponent>(&mData, houseActor);
         m_allActors.push_back(houseActor);
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
         CreateAndAddComponent<SkyboxComponent>(&mData, skyboxActor);
         m_allActors.push_back(skyboxActor);
      }
   }

   template <typename PrimitiveType>
   void Scene::CreateAndAddComponent(ComponentData* componentData, Actor* addComponentToThisActor)
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
            componentPtr->SceneProxyComponentId = TotalSceneComponentIndex++;
            auto sceneProxyUnique = std::move(componentPtr->CreateSceneProxy());
            m_sceneProxies.push_back(sceneProxyUnique);
         }
      }

      addComponentToThisActor->AddComponent(component);
   }

   void Scene::RemoveComponent(std::shared_ptr<Component> component)
   {
      ComponentType type = component->GetComponentType();

      // Remove corresponding primitive proxy
      if (type & ComponentType::PRIMITIVE_COMPONENT)
      {
         PrimitiveComponent* componentPtr = static_cast<PrimitiveComponent*>(component.get());
         const size_t removeProxyIndex = componentPtr->SceneProxyComponentId;
         std::shared_ptr<PrimitiveSceneProxy> removeProxy = m_sceneProxies.at(removeProxyIndex);
      
         // Remove proxy index offset
         for (auto& actor : m_allActors)
         {
            actor->RemoveComponentIndexOffset(removeProxyIndex);
         }

         auto proxyIt = std::find(m_sceneProxies.begin(), m_sceneProxies.end(), removeProxy);
         if (proxyIt != m_sceneProxies.end())
         {
            m_sceneProxies.erase(proxyIt);
            TotalSceneComponentIndex--;
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
      if (primitiveSceneProxyIndex < m_sceneProxies.size())
      {
         m_sceneProxies[primitiveSceneProxyIndex]->SetTransformationMatrix(newRelativeMatrix);
      }
   }

   void Scene::Render()
   {
      glEnable(GL_DEPTH_TEST);

      glm::mat4 viewMatrix = camera->GetViewMatrix();

      std::vector<std::shared_ptr<PrimitiveSceneProxy>> drawDeferredShadedPrimitives;
      std::vector<std::shared_ptr<PrimitiveSceneProxy>> drawForwardShadedPrimitives;

      for (auto& proxy : m_sceneProxies)
      {
         if (proxy->IsDeferred())
            drawDeferredShadedPrimitives.push_back(proxy);
         else
            drawForwardShadedPrimitives.push_back(proxy);
      }

      {
         // Deferred shading collect info
         m_deferredRenderer.PrepareDR();
         auto deferredShadingShader = m_deferredRenderer.GetDeferredShader();

         deferredShadingShader->ExecuteShader();
         for (auto& proxy : drawDeferredShadedPrimitives)
         {
            const glm::mat4& worldMatrix = proxy->GetMatrix();
            deferredShadingShader->SetTransformMatrices(worldMatrix, viewMatrix, projectionMatrix);

            proxy->GetAlbedo()->BindTexture(0);
            deferredShadingShader->SetAlbedoTextureSlot(0);

            proxy->GetSkin()->GetBuffer()->RenderVAO(GL_TRIANGLES);
         }
         deferredShadingShader->StopShader();

         m_deferredRenderer.StopDR();
      }

      {
         // Forward shading
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glClearColor(0, 0, 0, 0);

         m_resolveTexShader->ExecuteShader();
         m_deferredRenderer.BindNormalTexture(0);
         m_resolveTexShader->SetTextureSlot(0);
         ScreenQuad::GetInstance()->GetBuffer()->RenderVAO(GL_TRIANGLES);
         m_resolveTexShader->StopShader();
      }
   }

   void Scene::Tick(float delta)
   {
      for (auto& actor : m_allActors)
      {
         actor->Tick(0.05f);
      }
   }

   void Scene::CameraRotate()
   {
      camera->Rotate();
   }

   Scene::~Scene()
   {
      for (Actor* actor : m_allActors)
      {
         delete actor;
      }
      m_allActors.clear();
   }

}
