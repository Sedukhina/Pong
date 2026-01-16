#include "Generated.h"
#include <string>
#include <sstream>
#include <magic_enum.hpp>

enum GeneratedMeshesTypes
{
	Circle
};

std::filesystem::path GeneratePathForCircle(float Radius, int Segments)
{
	std::filesystem::path AbsPath(std::string(magic_enum::enum_name(Circle)));
	AbsPath += "_";
	AbsPath += std::to_string(Radius);
	AbsPath += "_";
	AbsPath += std::to_string(Segments);
	AbsPath += GeneratedMeshExtension;
	return AbsPath;
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateVerticesAndIndices(std::filesystem::path path)
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int > Indices;

	std::string StringPath = path.stem().string();

	if (StringPath.empty())
	{
		return std::pair<std::vector<Vertex>, std::vector<unsigned int>>{Vertices, Indices};
	}

	// Getting type and all variables
	std::vector<std::string> Tokens;
	std::stringstream ss(StringPath);
	std::string Token;
	while (std::getline(ss, Token, '_')) 
	{
		Tokens.push_back(Token);
	}

	return std::pair<std::vector<Vertex>, std::vector<unsigned int>>{Vertices, Indices};
}
