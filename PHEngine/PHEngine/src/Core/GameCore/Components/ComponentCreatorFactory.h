#pragma once

#include <memory>

#include "Component.h"
#include "SkyboxComponent.h"
#include "StaticMeshComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "BillboardComponent.h"
#include "CubemapComponent.h"
#include "ComponentType.h"
#include "Core/ResourceManagerCore/Pool/TexturePool.h"
#include "Core/ResourceManagerCore/Pool/ShaderPool.h"
#include "Core/ResourceManagerCore/Pool/MeshPool.h"
#include "Core/ResourceManagerCore/Pool/AnimationPool.h"
#include "Core/ResourceManagerCore/Pool/SimplePrimitivePool.h"
#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GameCore/Components/ComponentData/SkyboxComponentData.h"
#include "Core/GameCore/Components/ComponentData/StaticMeshComponentData.h"
#include "Core/GameCore/Components/ComponentData/DirectionalLightComponentData.h"
#include "Core/GameCore/Components/ComponentData/PointLightComponentData.h"
#include "Core/GameCore/Components/ComponentData/SkeletalMeshComponentData.h"
#include "Core/GameCore/Components/ComponentData/CubemapComponentData.h"
#include "Core/GameCore/Components/WaterPlaneComponent.h"
#include "Core/GameCore/Components/ComponentData/WaterPlaneComponentData.h"
#include "Core/GameCore/Components/ComponentData/MovementComponentData.h"
#include "Core/GameCore/Components/ComponentData/BillboardComponentData.h"
#include "Core/GameCore/Components/SkeletalMeshComponent.h"
#include "Core/GameCore/ShaderImplementation/SkeletalMeshShader.h"
#include "Core/GameCore/ShaderImplementation/CubemapShader.h"
#include "Core/CommonCore/CommonMeta.h"

using namespace Resources;
using namespace Common;

namespace Game
{
   template <typename ComponentType, typename MaterialType = NullType>
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

            int32_t primitive = (int32_t)SimplePrimitiveType::CUBE;
            auto skin = SimplePrimitivePool::GetInstance()->GetOrAllocateResource(primitive);

            typename TexturePool::sharedValue_t dTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesDay);
            typename TexturePool::sharedValue_t nTex;
            if (mData.m_commaSeparatedPathToSixTexturesNight != "")
               nTex = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_commaSeparatedPathToSixTexturesNight);

            ShaderParams shaderParams("Skybox Shader", mData.m_vsShaderPath, mData.m_fsShaderPath, "", "", "", "");
            ShaderPool::sharedValue_t skyboxShader = ShaderPool::GetInstance()->template GetOrAllocateResource<SkyboxShader>(shaderParams);

            SkyboxRenderData renderData(skin, skyboxShader, dTex, nTex);

            resultComponent = std::make_shared<SkyboxComponent>(mData.m_scale, mData.m_rotateSpeed, renderData);
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

            ShaderParams shaderParams("StaticMesh Shader", mData.m_vsShaderPath, mData.m_fsShaderPath, "", "", "", "");
            ShaderPool::sharedValue_t staticMeshShader = ShaderPool::GetInstance()->template GetOrAllocateResource<StaticMeshShader>(shaderParams);

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
   template <typename MaterialType>
   struct ComponentCreatorFactory<SkeletalMeshComponent, MaterialType>
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

            //////////////////////////////////////////////////////////////////////////

     /*       using texShared = PBRMaterial::ITextureShared;
            std::string diffuseTexPath = folderManager->GetAlbedoTexturePath() + "diffuse.png";
            std::string nmPath = folderManager->GetNormalMapPath() + "dummy_nm.png";
            texShared image = TexturePool::GetInstance()->GetOrAllocateResource(diffuseTexPath);
            texShared imageNM = TexturePool::GetInstance()->GetOrAllocateResource(nmPath);
            mMatSkelet = std::make_shared<PBRMaterial>(image, imageNM, imageNM, imageNM, imageNM);

            using skeletShader_t = CompositeShader<SkeletalMeshVertexFactory<3>, DeferredCollectShader, MaterialShaderImp<MaterialType>>;

            const auto shParams = ShaderParams("DeferredNonSkeletalBase Shader", FolderManager::GetInstance()->GetShadersPath() + "composite_shaders\\" + "deferredBaseVS.glsl", FolderManager::GetInstance()->GetShadersPath() + "composite_shaders\\" + "deferredBaseFS.glsl", "", "", "", "");

            TemplatedCompositeShaderParams<skeletShader_t> compositeParams(COMPOSITE_SHADER_TO_STR(SkeletalMeshVertexFactory<3>, DeferredCollectShader, MaterialShaderImp<MaterialType>), shParams);

            mTestShaderSkelet = std::static_pointer_cast<skeletShader_t>(CompositeShaderPool::GetInstance()->GetOrAllocateResource<skeletShader_t>(compositeParams));*/

            //////////////////////////////////////////////////////////////////////////

            ShaderParams shaderParams("SkeletalMesh Shader", mData.m_vsShaderPath, mData.m_fsShaderPath, "", "", "", "");
            ShaderPool::sharedValue_t skeletalMeshShader = ShaderPool::GetInstance()->template GetOrAllocateResource<SkeletalMeshShader>(shaderParams);

            SkeletalMeshRenderData renderData(skin, animations, std::make_shared<ICompositeShader>(), std::make_shared<IMaterial>(), skeletalMeshShader, albedo, normalMap, specularMap);
             
            resultComponent = std::make_shared<SkeletalMeshComponent>(std::move(mData.m_translation), std::move(mData.m_rotation), std::move(mData.m_scale), renderData);
         }

         return resultComponent;
      }

   };

   // Billboard component
   template<>
   struct ComponentCreatorFactory<BillboardComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;

         {
            BillboardComponentData& mData = static_cast<BillboardComponentData&>(data);

            int32_t primitive = (int32_t)SimplePrimitiveType::POINT;
            SimplePrimitivePool::sharedValue_t skin = SimplePrimitivePool::GetInstance()->GetOrAllocateResource(primitive);
            typename TexturePool::sharedValue_t texture = TexturePool::GetInstance()->GetOrAllocateResource(mData.m_pathToTexture);
            ShaderParams shaderParams("Billboard Shader", mData.m_vsShaderPath, mData.m_fsShaderPath, mData.m_gsShaderPath, "", "", "");
            ShaderPool::sharedValue_t shader = ShaderPool::GetInstance()->template GetOrAllocateResource<BillboardShader>(shaderParams);

            BillboardRenderData renderData(skin, shader, texture);

            resultComponent = std::make_shared<BillboardComponent>(std::move(mData.m_translation), std::move(mData.m_rotation), std::move(mData.m_scale), renderData);
         }

         return resultComponent;
      }
   };

   // Cubemap component
   template<>
   struct ComponentCreatorFactory<CubemapComponent>
   {
      static std::shared_ptr<Component> CreateComponent(ComponentData& data)
      {
         std::shared_ptr<Component> resultComponent;

         {
            CubemapComponentData& mData = static_cast<CubemapComponentData&>(data);

            int32_t primitive = (int32_t)SimplePrimitiveType::CUBE;
            SimplePrimitivePool::sharedValue_t skin = SimplePrimitivePool::GetInstance()->GetOrAllocateResource(primitive);
            ShaderParams shaderParams("Cubemap Shader", mData.m_vsShaderPath, mData.m_fsShaderPath, "", "", "", "");
            ShaderPool::sharedValue_t shader = ShaderPool::GetInstance()->template GetOrAllocateResource<CubemapShader>(shaderParams);

            CubemapRenderData renderData(skin, shader, mData.m_textureObtainer);

            resultComponent = std::make_shared<CubemapComponent>(std::move(mData.m_translation), std::move(mData.m_rotation), std::move(mData.m_scale), renderData);
         }

         return resultComponent;
      }
   };

}