#include "AssetManager.h"
#include "Assets/Asset.h"
#include "Log.h"
#include "Generated.h"

#include <assimp/scene.h>  
#include <assimp/Importer.hpp> 
#include <assimp/postprocess.h>  

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "Font.h"

stbtt_packedchar PackedChars[CharsToIncludeInFontAtlas];

#include <fstream>

const GLuint AssetManager::GetTextureAddress(std::filesystem::path Path, uint64_t ID)
{
	if (LoadedTextures.find(ID) == LoadedTextures.end())
	{
		LoadAsset(Path, ID);
	}
	if (LoadedTextures.find(ID) == LoadedTextures.end())
	{
		LOG_WARNING("GLuint AssetManager::GetTextureAddress: Provided path to asset doesn't exist: ");
		LOG_WARNING(Path.string().c_str());
		return 0;
	}
	return LoadedTextures[ID];
}

std::pair <const GLuint, const unsigned int> AssetManager::GetMeshAddressAndIndicesSize(std::filesystem::path Path, uint64_t ID)
{
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		LoadAsset(Path, ID);
	}
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		LOG_WARNING("GLuint AssetManager::GetMeshAddress: Provided path to asset doesn't exist: ");
		LOG_WARNING(Path.string().c_str());
		return { 0, 0 };
	}
	return { LoadedMeshes[ID]->GetVAO(), LoadedMeshes[ID]->GetIndicesArraySize() };
}

std::array<glm::vec2, 2> AssetManager::GetMeshAABB(std::filesystem::path Path, uint64_t ID)
{
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		LoadAsset(Path, ID);
	}
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		return std::array<glm::vec2, 2>();
	}
	return std::array<glm::vec2, 2>{LoadedMeshes[ID]->GetAABBMin(), LoadedMeshes[ID]->GetAABBMax()};
}

GLenum AssetManager::GetMeshDrawingMode(uint64_t ID)
{
	if (LoadedMeshes.find(ID) == LoadedMeshes.end())
	{
		return GL_TRIANGLES;
	}
	return LoadedMeshes[ID]->GetDrawingMode();
}

void AssetManager::LoadAsset(std::filesystem::path Path, uint64_t ID)
{
	AssetType Type = GetAssetType(Path);
	if (Type == AssetType::GeneratedMesh)
	{
		LoadGeneratedMesh(Path, ID);
		return;
	}
	if (!IsExistingPath(&Path))
	{
		LOG_WARNING("AssetManager::LoadAsset: Provided path to asset doesn't exist: ");
		LOG_WARNING(Path.string().c_str());
		throw std::runtime_error("AssetManager::LoadAsset: Provided path to asset doesn't exist: " + Path.string());
		return;
	}
	if (Type == AssetType::Texture)
	{
		LoadTexture(Path, ID);
	}
	else if (Type == AssetType::Model)
	{
		LoadModel(Path, ID);
	}
	else
	{
		LOG_WARNING("Provided path directs to unknown asset type: ");
		LOG_WARNING(Path.string().c_str());
		throw std::runtime_error("Provided path directs to unknown asset type: " + Path.string());
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
	for (const std::pair <const uint64_t, GLuint> Tex : LoadedTextures)
	{
		glDeleteTextures(1, &Tex.second);
	}
}
	
void AssetManager::LoadFont()
{
	// READING FONT FILE
	std::ifstream InputFileStream(FontPath, std::ios::binary);
	// Find the size of the file to allocate memory dynamically
	InputFileStream.seekg(0, std::ios::end);
	auto&& FileSize = InputFileStream.tellg();
	InputFileStream.seekg(0, std::ios::beg);
	// Allocate the buffer
	uint8_t* FontDataBuf = new uint8_t[static_cast<size_t>(FileSize)];
	// Read the font data to the buffer
	InputFileStream.read((char*)FontDataBuf, FileSize);

	uint8_t* FontAtlasBitmap = new uint8_t[FontAtlasWidth * FontAtlasHeight];

	stbtt_aligned_quad AlignedQuads[CharsToIncludeInFontAtlas];
	
	stbtt_pack_context PackContext;
	// Context, Atlas data, Atlas texture width and height, stride in bytes, padding between glyphs
	stbtt_PackBegin(&PackContext, (unsigned char*)FontAtlasBitmap,  FontAtlasWidth, FontAtlasHeight, 0, 1, nullptr);
	// Context, Atlas tex data, font index, font size, characters to include (first, amount), struct for glyph render
	stbtt_PackFontRange(&PackContext, FontDataBuf, 0, FontSize, CodePointOfFirstChar, CharsToIncludeInFontAtlas, PackedChars);
	stbtt_PackEnd(&PackContext);
	
	for (int GlyphIndex = 0; GlyphIndex < CharsToIncludeInFontAtlas; GlyphIndex++)
	{
		float x = 0.f;
		float y = 0.f;
		stbtt_GetPackedQuad(PackedChars, FontAtlasWidth,FontAtlasHeight, GlyphIndex, &x, &y, &AlignedQuads[GlyphIndex], 0);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &EnginesFontAtlas);
	glBindTexture(GL_TEXTURE_2D, EnginesFontAtlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, FontAtlasWidth, FontAtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, FontAtlasBitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_write_png("FontAtlas.png", FontAtlasWidth, FontAtlasHeight, 1, FontAtlasBitmap, FontAtlasWidth);

	delete[] FontAtlasBitmap;
	delete[] FontDataBuf;
}

GLuint AssetManager::GetFontAtlas() const
{
	return EnginesFontAtlas;
}

void AssetManager::GenTextUIVertices(std::string Text, GLuint VBO)
{
	std::vector<TextVertex> Vertices;
	Vertices.reserve(Text.size() * 4);

	float X = 0.f;
	float Y = 0.f;

	for (size_t i = 0; i < Text.size(); ++i) {
		char Character = Text[i];
		if (Character < CodePointOfFirstChar || Character >= CodePointOfFirstChar + CharsToIncludeInFontAtlas)
		{
			continue;
		}

		stbtt_aligned_quad AllignedQuad;
		stbtt_GetPackedQuad(PackedChars, FontAtlasWidth, FontAtlasHeight, Character - CodePointOfFirstChar, &X, &Y, &AllignedQuad, 0);

		TextVertex BottomLeft;
		BottomLeft.PositionX = AllignedQuad.x0;
		BottomLeft.PositionY = AllignedQuad.y0;
		BottomLeft.TextureCoordinateX = AllignedQuad.s0;
		BottomLeft.TextureCoordinateY = AllignedQuad.t1;

		TextVertex TopLeft;
		TopLeft.PositionX = AllignedQuad.x0;
		TopLeft.PositionY = AllignedQuad.y1;
		TopLeft.TextureCoordinateX = AllignedQuad.s0;
		TopLeft.TextureCoordinateY = AllignedQuad.t0;

		TextVertex BottomRight;
		BottomRight.PositionX = AllignedQuad.x1;
		BottomRight.PositionY = AllignedQuad.y0;
		BottomRight.TextureCoordinateX = AllignedQuad.s1;
		BottomRight.TextureCoordinateY = AllignedQuad.t1;

		TextVertex TopRight;
		TopRight.PositionX = AllignedQuad.x1;
		TopRight.PositionY = AllignedQuad.y1;
		TopRight.TextureCoordinateX = AllignedQuad.s1;
		TopRight.TextureCoordinateY = AllignedQuad.t0;

		Vertices.push_back(BottomLeft);
		Vertices.push_back(TopLeft);
		Vertices.push_back(BottomRight);
		Vertices.push_back(TopRight);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Vertices.size() * sizeof(TextVertex), &Vertices[0]);
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
		throw std::runtime_error("Failed to load texture file " + path.string());
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

void AssetManager::LoadGeneratedMesh(std::filesystem::path path, uint64_t ID)
{
	glm::vec2 AABBMin;
	glm::vec2 AABBMax;
	GLenum DrawingMode;

	std::pair<std::vector<Vertex>, std::vector<unsigned int>> VerticesAndIndices = GenerateVerticesAndIndices(path, &AABBMin, &AABBMax, &DrawingMode);

	LoadMeshToVideomemory(VerticesAndIndices.first, VerticesAndIndices.second, AABBMin, AABBMax, ID, DrawingMode);
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

	LoadMeshToVideomemory(Vertices, Indices, glm::vec2(AiMesh->mAABB.mMin.x, AiMesh->mAABB.mMin.y), glm::vec2(AiMesh->mAABB.mMax.x, AiMesh->mAABB.mMax.y), ID);
}

void AssetManager::LoadMeshToVideomemory(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices, glm::vec2 AABBmin, glm::vec2 AABBmax, uint64_t ID, GLenum DrawingMode)
{
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
			AABBmin,
			AABBmax));

	LoadedMeshes[ID]->SetDrawingMode(DrawingMode);
	glBindVertexArray(0);
}
