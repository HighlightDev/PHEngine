#pragma once

#include "Core/GameCore/Components/ComponentData/ComponentData.h"
#include "Core/GameCore/Components/ComponentType.h"

#include <string>

namespace Game
{
	struct SkyboxComponentData : public ComponentData
	{
		SkyboxComponentData(float skyboxSideSize, float rotateSpeed, std::string&& vsPath,
			std::string&& fsPath, std::string&& commaSeparatedPathToSixTexturesDay, std::string&& commaSeparatedPathToSixTexturesNight = "")

			: ComponentData()
			, m_rotateSpeed(rotateSpeed)
			, m_skyboxSideSize(skyboxSideSize)
			, m_vsShaderPath(std::move(vsPath))
			, m_fsShaderPath(std::move(fsPath))
			, m_commaSeparatedPathToSixTexturesDay(std::move(commaSeparatedPathToSixTexturesDay))
			, m_commaSeparatedPathToSixTexturesNight(std::move(commaSeparatedPathToSixTexturesNight))
		{

		}

		virtual size_t GetType() override {

			return SKYBOX_COMPONENT;
		}

		float m_rotateSpeed;
		float m_skyboxSideSize;
		std::string m_commaSeparatedPathToSixTexturesDay;
		std::string m_commaSeparatedPathToSixTexturesNight;
		std::string m_vsShaderPath;
		std::string m_fsShaderPath;
	};

}