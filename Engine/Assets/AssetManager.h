#pragma once
#include <assimp/scene.h>    
#include <filesystem>
#include <map>
#include <memory>
#include <array>
#include "Mesh.h"

class AssetManager
{
public:
	AssetManager() {};
	
	const GLuint GetTextureAddress(std::filesystem::path path, uint64_t ID);
	std::pair<const GLuint, const unsigned int> GetMeshAddressAndIndicesSize(std::filesystem::path path, uint64_t ID);
	std::array<glm::vec2, 2> GetMeshAABB(uint64_t ID);

	void LoadAsset(std::filesystem::path path, uint64_t ID);

	~AssetManager();

private:
	void LoadTexture(std::filesystem::path path, uint64_t ID);
	void LoadModel(std::filesystem::path path, uint64_t ID);
	// Returns if anything was loaded
	void LoadFromAssimpScene(const aiScene* Scene, uint64_t ID);

	std::map<uint64_t, std::unique_ptr<Mesh>> LoadedMeshes;
	std::map<uint64_t, GLuint> LoadedTextures;
};
