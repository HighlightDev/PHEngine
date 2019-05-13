#pragma once
#include "CameraBase.h"
#include "Actor.h"

namespace Game
{
   class ThirdPersonCamera :
      public CameraBase
   {

      float m_distanceFromTargetToCamera;

      Actor* m_thirdPersonTarget;

      glm::vec3 m_actualTargetVector;

      float m_lerpTimeElapsed = 0.0f;

      float m_timeForInterpolation = 0.15f;

      bool m_bThirdPersonTargetTransformationDirty = false;

   public:

      float m_maxDistanceFromTargetToCamera;

      ThirdPersonCamera();

      ThirdPersonCamera(glm::vec3 localSpaceForwardVector, float camDistanceToThirdPersonTarget);

      virtual ~ThirdPersonCamera();

      virtual void Tick(float DeltaTime) override;

      void SetMaxDistanceFromTargetToCamera(float maxDistanceFromTargetToCamera);

      float GetMaxDistanceFromTargetToCamera() const;

      float GetTimeForInterpolation() const;

      void SetTimeForInterpolation(float timeForInterpolation);

      virtual glm::vec3 GetLocalSpaceUpVector() const override;

      virtual glm::vec3 GetEyeVector() const override;

      virtual glm::vec3 GetTargetVector() const override;

      void SetDistanceFromTargetToCamera(float distanceFromTargetToCamera);

      float GetDistanceFromTargetToCamera() const;

      Actor* GetThirdPersonTarget() const;

      void SetThirdPersonTarget(Actor* thirdPersonTarget);

      void SetThirdPersonTargetTransformationDirty();
   };
}

