#pragma once

#include <memory>

#include "Component.h"
#include "SkyboxComponent.h"
#include "StaticMeshComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "ComponentType.h"
#include "Core/ResourceManagerCore/Pool/TexturePool.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"
#include "Core/ResourceManagerCore/Pool/MeshPool.h"
#include "Core/ResourceManagerCore/Pool/AnimationPool.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GameCore/Components/ComponentData/SkyboxComponentData.h"
#include "Core/GameCore/Components/ComponentData/StaticMeshComponentData.h"
#include "Core/GameCore/Components/ComponentData/DirectionalLightComponentData.h"
#include "Core/GameCore/Components/ComponentData/PointLightComponentData.h"
#include "Core/GameCore/Components/ComponentData/SkeletalMeshComponentData.h"
#include "Core/GameCore/Components/WaterPlaneComponent.h"
#include "Core/GameCore/Components/ComponentData/WaterPlaneComponentData.h"
#include "Core/GameCore/Components/ComponentData/MovementComponentData.h"
#include "Core/GameCore/Components/SkeletalMeshComponent.h"
#include "Core/GameCore/ShaderImplementation/SkeletalMeshShader.h"

using namespace Resources;

namespace Game
{
   template <typename T>
   struct ComponentCreatorFactory;

   // Skybox component
   template <>
   struct ComponentCreatorFactory<SkyboxComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;
         if (data.GetType() == SKYBOX_COMPONENT)
         {
            SkyboxComponentData& mData = static_cast<SkyboxComponentData&>(data);

            const float skyboxSize = mData.m_skyboxSideSize;

            std::vector<float> vertices = { -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, -skyboxSize ,
               skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, skyboxSize ,
               -skyboxSize, -skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, skyboxSize, skyboxSize ,
               -skyboxSize, -skyboxSize, skyboxSize , skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, skyboxSize , skyboxSize, skyboxSize, skyboxSize ,
               skyboxSize, skyboxSize, skyboxSize , skyboxSize, skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, -skyboxSize, -skyboxSize, -skyboxSize, skyboxSize ,
               -skyboxSize, skyboxSize, skyboxSize , skyboxSize, skyboxSize, skyboxSize , skyboxSize, skyboxSize, skyboxSize , skyboxSize, -skyboxSize, skyboxSize ,
               -skyboxSize, -skyboxSize, skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , skyboxSize, skyboxSize, -skyboxSize , skyboxSize, skyboxSize, skyboxSize ,
               skyboxSize, skyboxSize, skyboxSize , -skyboxSize, skyboxSize, skyboxSize , -skyboxSize, skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, -skyboxSize ,
               -skyboxSize, -skyboxSize, skyboxSize , skyboxSize, -skyboxSize, -skyboxSize , skyboxSize, -skyboxSize, -skyboxSize , -skyboxSize, -skyboxSize, skyboxSize ,
               skyboxSize, -skyboxSize, skyboxSize };

            std::unique_ptr<VertexArrayObject> vao = std::make_unique<VertexArrayObject>();
            VertexBufferObjectBase* vbo = new VertexBufferObject<float, 3, GL_FLOAT>(std::make_shared<std::vector<float>>(vertices), GL_ARRAY_BUFFER, 0, DataCarryFlag::Invalidate);
            vao->AddVBO(std::unique_ptr<VertexBufferObjectBase>(std::move(vbo)));

            vao->BindBuffersToVao();

            auto skin = std::make_shared<Skin>(std::move(vao));

            typename TexturePool::sharedValue_t dTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesDay);
            typename TexturePool::sharedValue_t nTex;
            if (mData.m_commaSeparatedPathToSixTexturesNight != "")
               nTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesNight);

            std::string shaderPath = std::move(mData.m_vsShaderPath) + "," + std::move(mData.m_fsShaderPath);
            ShaderPool::sharedValue_t skyboxShader = ShaderPool::GetInstance()->template GetOrAllocateResource<SkyboxShader>(shaderPath);

            SkyboxRenderData renderData(skin, skyboxShader, dTex, nTex);

            resultComponent = std::make_shared<SkyboxComponent>(mData.m_rotateSpeed, renderData);
         }

         return resultComponent;
      }
   };

   //Static Mesh component
   template <>
   struct ComponentCreatorFactory<StaticMeshComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;
         if (data.GetType() == STATIC_MESH_COMPONENT)
         {
            StaticMeshComponentData& mData = static_cast<StaticMeshComponentData&>(data);

            typename MeshPool::sharedValue_t skin = MeshPool::GetInstance()->GetOrAllocateResource(mData.m_pathToMesh);

            typename TexturePool::sharedValue_t albedo = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToAlbedo);
            typename TexturePool::sharedValue_t normalMap;
            typename TexturePool::sharedValue_t specularMap;

            if (mData.m_pathToNormalMap != "")
               normalMap = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToNormalMap);

            if (mData.m_pathToSpecularMap != "")
               specularMap = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToSpecularMap);

            std::string shaderPath = std::move(mData.m_vsShaderPath) + "," + std::move(mData.m_fsShaderPath);
            ShaderPool::sharedValue_t staticMeshShader = ShaderPool::GetInstance()->template GetOrAllocateResource<StaticMeshShader>(shaderPath);

            StaticMeshRenderData renderData(skin, staticMeshShader, albedo, normalMap, specularMap);

            resultComponent = std::make_shared<StaticMeshComponent>(std::move(mData.m_translation), std::move(mData.m_rotation), std::move(mData.m_scale), renderData);
         }

         return resultComponent;
      }
   };

   // Directional light component
   template <>
   struct ComponentCreatorFactory<DirectionalLightComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;
         if (data.GetType() == DIR_LIGHT_COMPONENT)
         {
            DirectionalLightComponentData& mData = static_cast<DirectionalLightComponentData&>(data);
            DirectionalLightRenderData renderData(mData.Direction, mData.Ambient, mData.Diffuse, mData.Specular, mData.ShadowInfo);
            resultComponent = std::make_shared<DirectionalLightComponent>(std::move(mData.Rotation), renderData);
         }

         return resultComponent;
      }
   };

   // Point light component
   template <>
   struct ComponentCreatorFactory<PointLightComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;
         if (data.GetType() == POINT_LIGHT_COMPONENT)
         {
            PointLightComponentData& mData = static_cast<PointLightComponentData&>(data);
            PointLightRenderData renderData(mData.Attenuation, mData.RadianceSqrRadius, mData.Ambient, mData.Diffuse, mData.Specular, mData.ShadowInfo);
            resultComponent = std::make_shared<PointLightComponent>(std::move(mData.Translation), std::move(mData.Rotation), renderData);
         }

         return resultComponent;
      }
   };

   // Water plane component
   template <>
   struct ComponentCreatorFactory<WaterPlaneComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {

      }
   };

   // Input component
   template <>
   struct ComponentCreatorFactory<InputComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         return std::make_shared<InputComponent>();
      }
   };

   // Movement component 
   template <>
   struct ComponentCreatorFactory<MovementComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;
         if (data.GetType() == MOVEMENT_COMPONENT)
         {
            MovementComponentData& mData = static_cast<MovementComponentData&>(data);
            resultComponent = std::make_shared<MovementComponent>(mData.m_camera, mData.m_launchVelocity);
         }

         return resultComponent;
      }
   };

   // Skeletal mesh component
   template <>
   struct ComponentCreatorFactory<SkeletalMeshComponent>
   {

      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;

         if (data.GetType() == SKELETAL_MESH_COMPONENT)
         {
            SkeletalMeshComponentData& mData = static_cast<SkeletalMeshComponentData&>(data);

            typename MeshPool::sharedValue_t skin = MeshPool::GetInstance()->GetOrAllocateResource(mData.m_pathToMesh);

            typename TexturePool::sharedValue_t albedo = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToAlbedo);
            typename TexturePool::sharedValue_t normalMap;
            typename TexturePool::sharedValue_t specularMap;

            typename AnimationPool::sharedValue_t animations = AnimationPool::GetInstance()->GetOrAllocateResource(mData.m_pathToMesh);

            if (mData.m_pathToNormalMap != "")
               normalMap = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToNormalMap);

            if (mData.m_pathToSpecularMap != "")
               specularMap = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToSpecularMap);

            std::string shaderPath = std::move(mData.m_vsShaderPath) + "," + std::move(mData.m_fsShaderPath);
            ShaderPool::sharedValue_t skeletalMeshShader = ShaderPool::GetInstance()->template GetOrAllocateResource<SkeletalMeshShader>(shaderPath);

            SkeletalMeshRenderData renderData(skin, animations, skeletalMeshShader, albedo, normalMap, specularMap);
             
            resultComponent = std::make_shared<SkeletalMeshComponent>(std::move(mData.m_translation), std::move(mData.m_rotation), std::move(mData.m_scale), renderData);
         }

         return resultComponent;
      }

   };

}