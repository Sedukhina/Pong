#include "Asset.h"

#include "Log.h"

const std::vector<std::string> ModelsExtensions = { ".fbx", ".obj" };
const std::vector<std::string> TexturesExtensions = { ".png", ".jpg" };

// Asset folder path conversion from Cmake variable
#define STRING(x) #x
#define TO_STRING(x) STRING(x)

uint64_t GetAssetID(std::filesystem::path path) 
{
	IsExistingPath(&path);
	uint64_t AssetID = std::hash<std::filesystem::path>{}(path.string());
	return AssetID;
}

// Helper function to choose correcrt loading path
AssetType GetAssetType(std::filesystem::path path)
{
	std::string Extension = path.extension().string();
	for (int i = 0; i < TexturesExtensions.size(); i++)
	{
		if (!Extension.compare(TexturesExtensions[i]))
		{
			return AssetType::Texture;
		}
	}
	for (int i = 0; i < ModelsExtensions.size(); i++)
	{
		if (!Extension.compare(ModelsExtensions[i]))
		{
			return AssetType::Model;
		}
	}
	return AssetType::None;
}

bool IsExistingPath(std::filesystem::path* path)
{
	// Checking path in case it's absolute
	if (!std::filesystem::exists(path->string()))
	{
		// Checking path in case it's realtive tp engine asset dir
		std::string root(TO_STRING(ASSETS_DIR));
		std::filesystem::path AbsPath(root);
		AbsPath += path->string();
		if (!std::filesystem::exists(AbsPath))
		{
			return false;
		}
		path->swap(AbsPath);
	}
	return true;
}