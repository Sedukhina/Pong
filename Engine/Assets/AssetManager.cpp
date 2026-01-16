#include "AssetManager.h"
#include "Assets/Asset.h"
#include "Log.h"

#include <assimp/Importer.hpp> 
#include <assimp/postprocess.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Vertex
{
	float PositionX;
	float PositionY;
	float PositionZ;

	float NormalX;
	float NormalY;
	float NormalZ;

	float TextureCoordinateX;
	float TextureCoordinateY;
};

const GLuint AssetManager::GetTextureAddress(std::filesystem::path path, uint64_t ID)
{
	if (LoadedTextures.find(ID) == LoadedTextures.end())
	{
		LoadAsset(path, ID);
	}
	if (LoadedTextures.find(ID) == LoadedTextures.end())
	{
		LOG_WARNING("GLuint AssetManager::GetTextureAddress: Provided path to asset doesn't exist: ");
		LOG_WARNING(path.string().c_str());
		return 0;
	}
	return LoadedTextures[ID];
}

std::pair <const GLuint, const unsigned int> AssetManager::GetMeshAddressAndIndicesSize(std::filesystem::path path, uint64_t ID)
{
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		LoadAsset(path, ID);
	}
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		LOG_WARNING("GLuint AssetManager::GetMeshAddress: Provided path to asset doesn't exist: ");
		LOG_WARNING(path.string().c_str());
		return { 0, 0 };
	}
	return { LoadedMeshes[ID]->GetVAO(), LoadedMeshes[ID]->GetIndicesArraySize() };
}

std::array<glm::vec2, 2> AssetManager::GetMeshAABB(std::filesystem::path path, uint64_t ID)
{
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		LoadAsset(path, ID);
	}
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		return std::array<glm::vec2, 2>();
	}
	return std::array<glm::vec2, 2>{LoadedMeshes[ID]->GetAABBMin(), LoadedMeshes[ID]->GetAABBMax()};
}

void AssetManager::LoadAsset(std::filesystem::path path, uint64_t ID)
{
	if (!IsExistingPath(&path))
	{
		LOG_WARNING("AssetManager::LoadAsset: Provided path to asset doesn't exist: ");
		LOG_WARNING(path.string().c_str());
		return;
	}
	AssetType Type = GetAssetType(path);
	if (Type == AssetType::Texture)
	{
		LoadTexture(path, ID);
	}
	else if (Type == AssetType::Model)
	{
		LoadModel(path, ID);
	}
	else
	{
		LOG_WARNING("Provided path directs to unknown asset type: ");
		LOG_WARNING(path.string().c_str());
	}
}

AssetManager::~AssetManager()
{
	for (const std::pair<const uint64_t, std::unique_ptr<Mesh>>& CurMesh : LoadedMeshes)
	{
		GLuint VAO = CurMesh.second->GetVAO();
		glDeleteVertexArrays(1, &VAO);
		GLuint VBO = CurMesh.second->GetVBO();
		glDeleteBuffers(1, &VBO);
		GLuint EBO = CurMesh.second->GetEBO();
		glDeleteBuffers(1, &EBO);
	}
}
	

void AssetManager::LoadTexture(std::filesystem::path path, uint64_t ID)
{
	// Checking if texture already exists
	auto TexturesIter = LoadedTextures.find(ID);
	if (TexturesIter != LoadedTextures.end())
	{
		char buffer[300];
		sprintf(buffer, "Texture (%s) is already loaded.", path.u8string().c_str());
		LOG_INFO(buffer);
	}
	int Width, Height, Channels;
	stbi_uc* Pixels = stbi_load(path.string().c_str(), &Width, &Height, &Channels, 0);
	const std::vector<unsigned char> Img(Pixels, Pixels + Width * Height * Channels);
	const unsigned char* Image = &Img[0];

	// Checking if texture succesfully loaded to memory
	if (!Pixels)
	{
		LOG_ERROR("Failed to load texture file ");
		LOG_ERROR(path.string().c_str());
		return;
	}

	// Generating place in videomemory
	GLuint TextureLoc;
	glGenTextures(1, &TextureLoc);

	// Binding new texture address to work with it
	glBindTexture(GL_TEXTURE_2D, TextureLoc);

	// Setting parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Bounding texture to address
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (Channels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	}
	else if (Channels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image);
	}

	LoadedTextures[ID] = TextureLoc;
}

void AssetManager::LoadModel(std::filesystem::path path, uint64_t ID)
{
	// Checking if model already exists
	auto MeshesIter = LoadedMeshes.find(ID);
	if (MeshesIter != LoadedMeshes.end())
	{
		char buffer[300];
		sprintf(buffer, "Model (%s) is already loaded.", path.u8string().c_str());
		LOG_INFO(buffer);
	}

	// Creating assimp importer to read scene from file
	Assimp::Importer Import;
	const aiScene* Scene = Import.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenBoundingBoxes);

	// Checking if loaded Models are valid
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		LOG_ERROR(Import.GetErrorString());
		return;
	}

	LoadFromAssimpScene(Scene, ID);
}

void AssetManager::LoadFromAssimpScene(const aiScene* Scene, uint64_t ID)
{
	if (Scene->mNumMeshes == 0)
	{
		LOG_INFO("No meshes were found in provided model file");
		return;
	}
	aiMesh* AiMesh = Scene->mMeshes[0];

	std::vector<unsigned int> Indices;
	Indices.reserve(3 * AiMesh->mNumFaces);
	for (unsigned int i = 0; i < AiMesh->mNumFaces; i++)
	{
		aiFace face = AiMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			Indices.push_back(face.mIndices[j]);
	}

	// Vertices import
	std::vector<Vertex> Vertices;
	Vertices.reserve(AiMesh->mNumVertices);

	for (unsigned int i = 0; i < AiMesh->mNumVertices; i++)
	{
		Vertex Vert;

		Vert.PositionX = AiMesh->mVertices[i].x;
		Vert.PositionY = AiMesh->mVertices[i].y;
		Vert.PositionZ = AiMesh->mVertices[i].z;

		Vert.NormalX = AiMesh->mNormals[i].x;
		Vert.NormalY = AiMesh->mNormals[i].y;
		Vert.NormalZ = AiMesh->mNormals[i].z;

		if (AiMesh->mTextureCoords[0])
		{
			Vert.TextureCoordinateX = AiMesh->mTextureCoords[0][i].x;
			Vert.TextureCoordinateY = AiMesh->mTextureCoords[0][i].y;
		}

		Vertices.push_back(Vert);
	}

	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, NormalX));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TextureCoordinateX));

	LoadedMeshes.emplace(ID, 
		std::make_unique<Mesh>(VAO, VBO, EBO, 
			Indices.size() * sizeof(unsigned int), 
			glm::vec2(AiMesh->mAABB.mMin.x, AiMesh->mAABB.mMin.y),
			glm::vec2(AiMesh->mAABB.mMax.x, AiMesh->mAABB.mMax.y)));
	glBindVertexArray(0);
}
