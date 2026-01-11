#pragma once

#include <filesystem>

class Model
{
public:
	Model(std::filesystem::path Mesh);
	Model(std::filesystem::path Mesh, std::filesystem::path Texture);
	~Model() {};

	const uint64_t GetMeshID() const;
	const uint64_t GetTextureID() const;

	const std::filesystem::path GetMeshPath() const;
	const std::filesystem::path GetTexturePath() const;

private:
	std::filesystem::path PathToMesh;
	uint64_t MeshID = 0;

	std::filesystem::path PathToTexture;
	uint64_t TextureID = 0;
};
