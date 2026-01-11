#include "AssetManager.h"
#include "Assets/Asset.h"
#include "Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

enum AssetType
{
	Texture,
	Model,
	None
};

#define STRING(x) #x
#define TO_STRING(x) STRING(x)

const std::vector<std::string> ModelsExtensions = { ".fbx", ".obj" };
const std::vector<std::string> TexturesExtensions = { ".png", ".jpg" };

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
			LOG_ERROR("Provided path to asset doesn't exist: ");
			LOG_ERROR(path->string().c_str());
			LOG_ERROR(AbsPath.string().c_str());
			return false;
		}
		path->swap(AbsPath);
	}
	return true;
}

const GLuint AssetManager::GetTextureAddress(std::filesystem::path path, uint64_t ID)
{
	if (LoadedTextures.find(ID) == LoadedTextures.end())
	{
		LoadAsset(path, ID);
	}
	if (LoadedTextures.find(ID) == LoadedTextures.end())
	{
		LOG_WARNING("Provided path to asset doesn't exist: ");
		LOG_WARNING(path.string().c_str());
		return 0;
	}
	return LoadedTextures[ID];
}

void AssetManager::LoadAsset(std::filesystem::path path, uint64_t ID)
{
	if (!IsExistingPath(&path))
	{
		LOG_WARNING("Provided path to asset doesn't exist: ");
		LOG_WARNING(path.string().c_str());
		return;
	}
	AssetType Type = GetAssetType(path);
	if (Type == AssetType::Texture)
	{
		LoadTexture(path, ID);
	}
	else
	{
		LOG_WARNING("Provided path directs to unknown asset type: ");
		LOG_WARNING(path.string().c_str());
	}
}

AssetManager::~AssetManager()
{
	/*for (const auto& [AssetID, AssetGLints] : LoadedMeshes)
	{
		glDeleteVertexArrays(1, &AssetGLints[0]);
		glDeleteBuffers(1, &AssetGLints[1]);
		glDeleteBuffers(1, &AssetGLints[2]);
	}*/
}
	

void AssetManager::LoadTexture(std::filesystem::path path, uint64_t ID)
{
	// Checking if texture already exists
	auto TexturesIter = LoadedTextures.find(GetAssetID(path));
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
	//std::vector<unsigned char> Img(Pixels, Pixels + Width * Height * Channels);
	//CTexture Texture = CTexture(Path, AssetName, Img, Height, Width, Channels);
	//Textures.emplace(Texture.GetAssetID(), std::make_unique<CTexture>(Texture));
	//return Texture.GetAssetID();
}
