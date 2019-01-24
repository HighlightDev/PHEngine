#include "FirstPersonCamera.h"

namespace Game
{

	FirstPersonCamera::FirstPersonCamera(glm::vec3 forwardVec, glm::vec3 camPos)
		: CameraBase()
		, m_firstPersonCameraPosition(camPos)
		, m_cameraMoveSpeed(1.5f)
	{
		m_eyeSpaceForwardVector = m_localSpaceForwardVector = forwardVec;
	}


	FirstPersonCamera::~FirstPersonCamera()
	{

	}

}