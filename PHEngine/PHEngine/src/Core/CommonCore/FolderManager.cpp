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

	std::string FolderManager::GetRootPath()
	{
		return m_rootFolder;
	}

	std::string FolderManager::GetResPath() 
	{
		return m_rootFolder + "res\\";
	}

	std::string FolderManager::GetModelPath()
	{
		return m_rootFolder + GetResPath() + "model\\";
	}

	std::string FolderManager::GetShadersPath()
	{
		return GetResPath() + "shaders\\";
	}

	std::string FolderManager::GetCollisionPath()
	{
		return GetResPath() + "collision\\";
	}

	std::string FolderManager::GetTexturesPath()
	{
		return GetResPath() + "texture\\";
	}

	std::string FolderManager::GetIniPath()
	{
		return GetResPath() + "ini\\";
	}

	std::string FolderManager::GetGrassTexturePath()
	{
		return GetTexturesPath() + "grass\\";
	}

	std::string FolderManager::GetHeightMapsTexturePath()
	{
		return GetTexturesPath() + "heightmaps\\";
	}

	std::string FolderManager::GetLandscapeTexturePath()
	{
		return GetTexturesPath() + "landscape\\";
	}

	std::string FolderManager::GetCubemapTexturePath()
	{
		return GetTexturesPath() + "cubemap\\";
	}

	std::string FolderManager::GetNormalMapPath()
	{
		return GetTexturesPath() + "normalmap\\";
	}

	std::string FolderManager::GetSpecularMapPath()
	{
		return GetTexturesPath() + "specularmap\\";
	}

	std::string FolderManager::GetAlbedoTexturePath()
	{
		return GetTexturesPath() + "albedo\\";
	}

	std::string FolderManager::GetDistortionTexturePath()
	{
		return GetTexturesPath() + "distortion\\";
	}

	std::string FolderManager::GetPostprocessTexturePath()
	{
		return GetTexturesPath() + "postprocess\\";
	}

	std::string FolderManager::GetEditorTexturePath()
	{
		return GetTexturesPath() + "editor\\";
	}
	
}
