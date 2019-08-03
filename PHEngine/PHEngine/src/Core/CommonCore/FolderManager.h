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

		std::string GetRootPath() const;
		std::string GetResPath() const;
		std::string GetModelPath() const;
		const std::string GetShadersPath() const;
      std::string GetShaderCommonPath() const;
		std::string GetCollisionPath() const;
		std::string GetTexturesPath() const;
		std::string GetIniPath() const;

		std::string GetGrassTexturePath() const;
		std::string GetHeightMapsTexturePath() const;
		std::string GetLandscapeTexturePath() const;
		std::string GetCubemapTexturePath() const;
		std::string GetNormalMapPath() const;
		std::string GetSpecularMapPath() const;
		std::string GetAlbedoTexturePath() const;
		std::string GetDistortionTexturePath() const;
		std::string GetPostprocessTexturePath() const;
		std::string GetEditorTexturePath() const;
	};
}

