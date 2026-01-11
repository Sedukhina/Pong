#include "Model.h"
#include "Assets/Asset.h"

Model::Model(std::filesystem::path Mesh)
	: PathToMesh(Mesh)
{
	MeshID = GetAssetID(Mesh);
}

Model::Model(std::filesystem::path Mesh, std::filesystem::path Texture)
	: PathToMesh(Mesh), PathToTexture(Texture)
{
	MeshID = GetAssetID(Mesh);
	TextureID = GetAssetID(Texture);
}

const uint64_t Model::GetMeshID() const
{
	return MeshID;
}

const uint64_t Model::GetTextureID() const
{
	return TextureID;
}

const std::filesystem::path Model::GetMeshPath() const
{
	return PathToMesh;
}

const std::filesystem::path Model::GetTexturePath() const
{
	return PathToTexture;
}
