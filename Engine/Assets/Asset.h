#pragma once
#include <filesystem>

enum class AssetType
{
	Texture,
	Model,
	GeneratedMesh,
	None
};


extern uint64_t GetAssetID(std::filesystem::path path);
extern AssetType GetAssetType(std::filesystem::path path);
extern bool IsExistingPath(std::filesystem::path* path);