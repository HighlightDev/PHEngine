#pragma once

#include "CameraBase.h"

namespace Game
{

	class FirstPersonCamera : public CameraBase
	{
	private:

		float m_cameraMoveSpeed;
		glm::vec3 m_firstPersonCameraPosition;

	public:

		FirstPersonCamera(glm::vec3 forwardVec, glm::vec3 camPos);

		~FirstPersonCamera();

		virtual void CameraTick(float DeltaTime) override {

		}

		virtual glm::vec3 GetEyeVector()  const override {

			return m_firstPersonCameraPosition;
		}

		virtual glm::vec3 GetTargetVector()  const override {

			auto targetVec = m_firstPersonCameraPosition + m_eyeSpaceForwardVector * m_cameraMoveSpeed;
			return targetVec;
		}

		virtual glm::vec3 GetLocalSpaceUpVector()  const override {

			return m_localSpaceUpVector;
		}

		inline float GetCameraMoveSpeed()  const {

			return m_cameraMoveSpeed;
		}

		void moveCamera(int32_t direction)
		{
			switch (direction)
			{
			case 0: m_firstPersonCameraPosition += GetEyeSpaceForwardVector() * m_cameraMoveSpeed; break;
			case 1: m_firstPersonCameraPosition -= GetEyeSpaceForwardVector() * m_cameraMoveSpeed; break;
			case 3: m_firstPersonCameraPosition -= GetEyeSpaceRightVector() * m_cameraMoveSpeed; break;
			case 4: m_firstPersonCameraPosition += GetEyeSpaceRightVector() * m_cameraMoveSpeed; break;
			}
		}
	};

}
