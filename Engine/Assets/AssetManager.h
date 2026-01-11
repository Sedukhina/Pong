#pragma once
#include <GL/glew.h>
#include <array>
#include <filesystem>
#include <map>

class AssetManager
{
public:
	AssetManager() {};
	
	const GLuint GetTextureAddress(std::filesystem::path path, uint64_t ID);

	void LoadAsset(std::filesystem::path path, uint64_t ID);

	~AssetManager();

private:
	void LoadTexture(std::filesystem::path path, uint64_t ID);

	// VAO, VBO and EBO  for each mesh
	std::map<uint64_t, std::array<GLint, 3>> LoadedMeshes;
	std::map<uint64_t, GLuint> LoadedTextures;
};
