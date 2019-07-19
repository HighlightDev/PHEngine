#include "ThirdPersonCamera.h"
#include <Core/UtilityCore/EngineMath.h>

#include <algorithm>

namespace Game
{

   ThirdPersonCamera::ThirdPersonCamera()
      : ICamera()
   {
      m_cameraType = ICamera::CameraType::THIRD_PERSON;
   }

   ThirdPersonCamera::ThirdPersonCamera(glm::vec3 localSpaceForwardVector, float camDistanceToThirdPersonTarget)
      : ICamera()
   {
      m_localSpaceForwardVector = m_eyeSpaceForwardVector = glm::normalize(localSpaceForwardVector);
      SetMaxDistanceFromTargetToCamera(camDistanceToThirdPersonTarget);
      m_distanceFromTargetToCamera = camDistanceToThirdPersonTarget;
      m_cameraType = ICamera::CameraType::THIRD_PERSON;
   }

   ThirdPersonCamera::~ThirdPersonCamera()
   {
   }

   void ThirdPersonCamera::Tick(float DeltaTime)
   {
      DeltaTime = std::max(DeltaTime, 0.03f);

      if (bTransformationDirty)
      {
         bTransformationDirty = false;
      }

      if (m_bThirdPersonTargetTransformationDirty)
      {
         m_lerpTimeElapsed = std::min(m_lerpTimeElapsed + DeltaTime, m_timeForInterpolation);

         glm::vec3 finalTargetVector = m_thirdPersonTarget->GetRootComponent()->GetTranslation();
         m_actualTargetVector = LerpPosition(m_lerpTimeElapsed, 0.0f, m_timeForInterpolation, m_actualTargetVector, finalTargetVector);

         // If camera is at final position  
         if (EngineUtility::CMP::Process(m_lerpTimeElapsed, m_timeForInterpolation))
         {
            m_lerpTimeElapsed = 0.0f;
            m_bThirdPersonTargetTransformationDirty = false;
         }
      }
   }

   void ThirdPersonCamera::SetMaxDistanceFromTargetToCamera(float maxDistanceFromTargetToCamera)
   {
      m_maxDistanceFromTargetToCamera = maxDistanceFromTargetToCamera;
      bTransformationDirty = true;
   }

   float ThirdPersonCamera::GetMaxDistanceFromTargetToCamera() const
   {
      return m_maxDistanceFromTargetToCamera;
   }

   float ThirdPersonCamera::GetTimeForInterpolation() const
   {
      return m_timeForInterpolation;
   }

   void ThirdPersonCamera::SetTimeForInterpolation(float timeForInterpolation)
   {
      m_timeForInterpolation = timeForInterpolation;
   }

   glm::vec3 ThirdPersonCamera::GetLocalSpaceUpVector() const
   {
      return m_localSpaceUpVector;
   }

   glm::vec3 ThirdPersonCamera::GetEyeVector() const
   {
      if (!m_thirdPersonTarget)
         return glm::vec3(0);

      return GetTargetVector() - (GetEyeSpaceForwardVector() * m_distanceFromTargetToCamera);
   }

   glm::vec3 ThirdPersonCamera::GetTargetVector() const
   {
      if (!m_thirdPersonTarget)
         return glm::vec3(1);

      return m_actualTargetVector;
   }

   void ThirdPersonCamera::SetDistanceFromTargetToCamera(float distanceFromTargetToCamera)
   {
      m_distanceFromTargetToCamera = distanceFromTargetToCamera;
   }

   float ThirdPersonCamera::GetDistanceFromTargetToCamera() const
   {
      return m_distanceFromTargetToCamera;
   }

   std::shared_ptr<Actor> ThirdPersonCamera::GetThirdPersonTarget() const
   {
      return m_thirdPersonTarget;
   }

   void ThirdPersonCamera::SetThirdPersonTarget(std::shared_ptr<Actor> thirdPersonTarget)
   {
      m_thirdPersonTarget = thirdPersonTarget;
      m_actualTargetVector = thirdPersonTarget->GetRootComponent()->GetTranslation();
   }

   void ThirdPersonCamera::SetThirdPersonTargetTransformationDirty()
   {
      m_lerpTimeElapsed = 0.0f;
      m_bThirdPersonTargetTransformationDirty = true;
   }

}
