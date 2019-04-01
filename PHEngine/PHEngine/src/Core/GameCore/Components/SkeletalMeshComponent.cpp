#include "SkeletalMeshComponent.h"

namespace Game
{

   SkeletalMeshComponent::SkeletalMeshComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, SkeletalMeshRenderData& renderData)
   {
   }

   SkeletalMeshComponent::~SkeletalMeshComponent()
   {
   }

   uint64_t SkeletalMeshComponent::GetComponentType() const override
   {

   }

   void SkeletalMeshComponent::Tick(float deltaTime) override
   {

   }

   std::shared_ptr<PrimitiveSceneProxy> SkeletalMeshComponent::CreateSceneProxy() const
   {

   }

}