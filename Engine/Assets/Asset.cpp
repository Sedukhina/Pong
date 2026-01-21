#include "Asset.h"
#include "Generated.h"
#include "Log.h"

const std::vector<std::string> ModelsExtensions = { ".fbx", ".obj" };
const std::vector<std::string> TexturesExtensions = { ".png", ".jpg" };

std::string AssetDirRoot = "../Assets/";

uint64_t GetAssetID(std::filesystem::path path) 
{
	if (GetAssetType(path) != AssetType::GeneratedMesh)
	{
		IsExistingPath(&path);
	}
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
	if (!Extension.compare(GeneratedMeshExtension))
	{
		return AssetType::GeneratedMesh;
	}
	return AssetType::None;
}

bool IsExistingPath(std::filesystem::path* path)
{
	if (!std::filesystem::exists(path->string()))
	{
		std::filesystem::path AbsPath(AssetDirRoot);
		AbsPath += path->string();
		if (!std::filesystem::exists(AbsPath))
		{
			return false;
		}
		path->swap(AbsPath);
	}
	return true;
}