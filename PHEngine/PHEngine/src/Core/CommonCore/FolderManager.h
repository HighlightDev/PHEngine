#pragma once
#include <string>
#include <memory>
#include <stdint.h>

#include "Core/UtilityCore/PlatformDependentFunctions.h"
#include "Core/UtilityCore/StringExtendedFunctions.h"

namespace Common
{

	class FolderManager
	{

	private:

		const std::string BINARY_FOLDER_NAME = "\\bin";

		std::string m_rootFolder;

		static std::shared_ptr<FolderManager> m_instance;

		std::string ConcatDirectoryBack(int32_t countChangeDirectoryBack);

		void BuildPathToFolders();

	public:

		FolderManager();

		~FolderManager();

		static std::shared_ptr<FolderManager> GetInstance()
		{
			if (m_instance == nullptr)
				m_instance = std::make_shared<FolderManager>();

			return m_instance;
		}

		std::string GetRootPath();
		std::string GetResPath();
		std::string GetModelPath();
		std::string GetShadersPath();
		std::string GetCollisionPath();
		std::string GetTexturesPath();
		std::string GetIniPath();

		std::string GetGrassTexturePath();
		std::string GetHeightMapsTexturePath();
		std::string GetLandscapeTexturePath();
		std::string GetCubemapTexturePath();
		std::string GetNormalMapPath();
		std::string GetSpecularMapPath();
		std::string GetAlbedoTexturePath();
		std::string GetDistortionTexturePath();
		std::string GetPostprocessTexturePath();
		std::string GetEditorTexturePath();
	};
}

