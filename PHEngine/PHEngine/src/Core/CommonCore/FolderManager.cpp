#include "FolderManager.h"

namespace Common
{

	std::shared_ptr<FolderManager> FolderManager::m_instance;

	FolderManager::FolderManager()
	{
		BuildPathToFolders();
	}


	FolderManager::~FolderManager()
	{
	}

	std::string FolderManager::ConcatDirectoryBack(int32_t countChangeDirectoryBack)
	{
		std::string resultStr = std::move(std::string(""));
		while (countChangeDirectoryBack-- > 0)
		{
			resultStr += "..\\";
		}
		return resultStr + "PHEngine\\";
	}

	void FolderManager::BuildPathToFolders()
	{
		// Root folder
		std::string currentDirPath = std::move(EngineUtility::GetExecutablePath());
		size_t indexOfRootEntrance = EngineUtility::IndexOf(currentDirPath, BINARY_FOLDER_NAME);
		std::string pathFromRootToCurrentDir = currentDirPath.substr(indexOfRootEntrance);

		size_t countChangeDirectoryBack = 0;
		for (auto& item : pathFromRootToCurrentDir) {
			countChangeDirectoryBack = item == '\\' ? ++countChangeDirectoryBack : countChangeDirectoryBack;
		}

		m_rootFolder = std::move(ConcatDirectoryBack(countChangeDirectoryBack));
	}

	std::string FolderManager::GetRootPath() const
	{
		return m_rootFolder;
	}

	std::string FolderManager::GetResPath() const
	{
		return GetRootPath() + "res\\";
	}

	std::string FolderManager::GetModelPath() const
	{
		return GetResPath() + "model\\";
	}

	const std::string FolderManager::GetShadersPath() const
	{
		return GetResPath() + "shaders\\";
	}

   std::string FolderManager::GetShaderCommonPath() const
   {
      return GetShadersPath() + "common\\";
   }

	std::string FolderManager::GetCollisionPath() const
	{
		return GetResPath() + "collision\\";
	}

	std::string FolderManager::GetTexturesPath() const
	{
		return GetResPath() + "texture\\";
	}

	std::string FolderManager::GetIniPath() const
	{
		return GetResPath() + "ini\\";
	}

	std::string FolderManager::GetGrassTexturePath() const
	{
		return GetTexturesPath() + "grass\\";
	}

	std::string FolderManager::GetHeightMapsTexturePath() const
	{
		return GetTexturesPath() + "heightmaps\\";
	}

	std::string FolderManager::GetLandscapeTexturePath() const
	{
		return GetTexturesPath() + "landscape\\";
	}

	std::string FolderManager::GetCubemapTexturePath() const
	{
		return GetTexturesPath() + "cubemap\\";
	}

	std::string FolderManager::GetNormalMapPath() const
	{
		return GetTexturesPath() + "normalmap\\";
	}

	std::string FolderManager::GetSpecularMapPath() const
	{
		return GetTexturesPath() + "specularmap\\";
	}

	std::string FolderManager::GetAlbedoTexturePath() const
	{
		return GetTexturesPath() + "albedo\\";
	}

	std::string FolderManager::GetDistortionTexturePath() const
	{
		return GetTexturesPath() + "distortion\\";
	}

	std::string FolderManager::GetPostprocessTexturePath() const
	{
		return GetTexturesPath() + "postprocess\\";
	}

	std::string FolderManager::GetEditorTexturePath() const
	{
		return GetTexturesPath() + "editor\\";
	}
	
   std::string FolderManager::GetPersistencyPath() const
   {
      return GetResPath() + "persistency\\";
   }
}
