#include "Asset.h"

uint64_t GetAssetID(std::filesystem::path path) 
{
	uint64_t AssetID = std::hash<std::filesystem::path>{}(path.string());
	return AssetID;
}
