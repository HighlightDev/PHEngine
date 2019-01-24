#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GlobalProperties.h"

namespace Game
{

	class CameraBase
	{
		float m_rotateSensetivity;

	protected:

		glm::vec3 m_localSpaceRightVector;
		glm::vec3 m_localSpaceUpVector;
		glm::vec3 m_localSpaceForwardVector;
		glm::vec3 m_eyeSpaceRightVector;
		glm::vec3 m_eyeSpaceForwardVector;
		glm::mat3 m_rotationMatrix;
		
		bool bTransformationDirty = false;

	private:

		void UpdateRotationMatrix(int32_t deltaX, int32_t deltaY);

	public:

		float CameraCollisionSphereRadius = 8.0f;

		CameraBase();

		virtual ~CameraBase();

		virtual void CameraTick(float DeltaTime) = 0;

		virtual glm::vec3 GetEyeVector() = 0;

		virtual glm::vec3 GetTargetVector() = 0;

		virtual glm::vec3 GetLocalSpaceUpVector() = 0;

		inline void SetLocalSpaceUpVector(glm::vec3& upVector)
		{
			m_localSpaceUpVector = upVector;
		}

		inline void SetLocalSpaceForwardVector(glm::vec3& forwardVector)
		{
			m_localSpaceForwardVector = forwardVector;
		}

		inline void SetLocalSpaceRightVector(glm::vec3& rightVector)
		{
			m_localSpaceRightVector = rightVector;
		}

		inline void SetCameraSensetivity(float rotateSensetivity)
		{
			m_rotateSensetivity = rotateSensetivity;
		}

		inline float GetCameraSensetivity()
		{
			return m_rotateSensetivity;
		}

		inline glm::vec3 GetLocalSpaceRightVector()
		{
			return m_localSpaceRightVector;
		}

		inline glm::vec3 GetLocalSpaceForwardVector()
		{
			return m_localSpaceForwardVector;
		}

		inline glm::vec3 GetEyeSpaceForwardVector()
		{
			return m_eyeSpaceForwardVector;
		}

		inline glm::vec3 GetEyeSpaceRightVector()
		{
			return m_eyeSpaceRightVector;
		}

		inline glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(GetEyeVector(), GetTargetVector(), GetLocalSpaceUpVector());
		}

		inline glm::mat3 GetRotationMatrix()
		{
			return m_rotationMatrix;
		}

		void Rotate();

		glm::vec3 LerpPosition(float t, float t1, float t2, glm::vec3 position1, glm::vec3 position2);

		/*inline FSphere GetCameraCollisionSphere()
		{
			return new FSphere(GetEyeVector(), CameraCollisionSphereRadius);
		}*/

		//void SetCollisionHeadUnit(CollisionHeadUnit collisionHeadUnit);
	};

}

