#include "FirstPersonCamera.h"

namespace Game
{

	FirstPersonCamera::FirstPersonCamera(glm::vec3 forwardVec, glm::vec3 camPos)
		: CameraBase()
		, m_firstPersonCameraPosition(camPos)
		, m_cameraMoveSpeed(0.1f)
	{
	}


	FirstPersonCamera::~FirstPersonCamera()
	{

	}

}