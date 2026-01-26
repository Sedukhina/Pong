#pragma once 
#include <filesystem>
#include <map>
#include <memory>
#include <array>
#include "Mesh.h"
#include "Vertex.h"

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager();
	
	const GLuint GetTextureAddress(std::filesystem::path Path, uint64_t ID);
	std::pair<const GLuint, const unsigned int> GetMeshAddressAndIndicesSize(std::filesystem::path path, uint64_t ID);
	std::array<glm::vec2, 2> GetMeshAABB(std::filesystem::path Path, uint64_t ID);
	GLenum GetMeshDrawingMode(uint64_t ID);

	void LoadFont();
	GLuint GetFontAtlas() const;
	void GenTextUIVertices(std::string Str, GLuint VBO);

	void LoadAsset(std::filesystem::path Path, uint64_t ID);

private:
	void LoadTexture(std::filesystem::path Path, uint64_t ID);
	void LoadModel(std::filesystem::path Path, uint64_t ID);
	void LoadGeneratedMesh(std::filesystem::path Path, uint64_t ID);
	void LoadFromAssimpScene(const class aiScene* Scene, uint64_t ID);
	void LoadMeshToVideomemory(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices, glm::vec2 AABBmin, glm::vec2 AABBmax, uint64_t ID, GLenum DrawingMode = GL_TRIANGLES);

	std::map<uint64_t, std::unique_ptr<Mesh>> LoadedMeshes;
	std::map<uint64_t, GLuint> LoadedTextures;
	GLuint EnginesFontAtlas = 0;
};
