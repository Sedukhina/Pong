#include "Generated.h"
#include <string>
#include <sstream>
#include <magic_enum.hpp>
#include <cmath>
#include "Log.h"

const float PI = 3.14;

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

std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateVerticesAndIndicesForCircle(float Radius, int Segments, glm::vec2* AABBmin, glm::vec2* AABBmax, GLenum* DrawingMode)
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int > Indices;

	AABBmin->x = -Radius;
	AABBmin->y = -Radius;
	AABBmax->x = Radius;
	AABBmax->y = Radius;

	*DrawingMode = GL_TRIANGLE_FAN;

	Vertex Center;
	Center.PositionX = 0;
	Center.PositionY = 0;
	Center.PositionZ = 0;
	Center.NormalX = 0;
	Center.NormalY = 0;
	Center.NormalZ = 0;
	Center.TextureCoordinateX = 0.5f;
	Center.TextureCoordinateY = 0.5f;
	Vertices.push_back(Center);

	for (int i = 0; i <= Segments; ++i)
	{
		float Angle = 2.0f * PI * i / Segments;
		Vertex Vert;
		Vert.NormalX = cosf(Angle);
		Vert.NormalY = sinf(Angle);
		Vert.NormalZ = 0.f;
		Vert.PositionX = Radius * Vert.NormalX;
		Vert.PositionY = Radius * Vert.NormalY;
		Vert.PositionZ = 0.f;
		Vert.TextureCoordinateX = 0.5f + 0.5f * Vert.NormalX;
		Vert.TextureCoordinateY = 0.5f + 0.5f * Vert.NormalY;
		Vertices.push_back(Vert);
	}

	for (int i = 0; i <= Segments+1; ++i)
	{
		Indices.push_back(i);
	}

	return std::pair<std::vector<Vertex>, std::vector<unsigned int>>{Vertices, Indices};
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateVerticesAndIndices(std::filesystem::path path, glm::vec2* AABBmin, glm::vec2* AABBmax, GLenum* DrawingMode)
{
	std::string StringPath = path.stem().string();

	if (StringPath.empty())
	{
		return std::pair<std::vector<Vertex>, std::vector<unsigned int>>{{}, {}};
	}

	// Getting type and all variables
	std::vector<std::string> Tokens;
	std::stringstream ss(StringPath);
	std::string Token;
	while (std::getline(ss, Token, '_')) 
	{
		Tokens.push_back(Token);
	}

	if (!Tokens[0].compare(magic_enum::enum_name(Circle)))
	{
		return GenerateVerticesAndIndicesForCircle(std::stof(Tokens[1]), std::stoi(Tokens[2]), AABBmin, AABBmax, DrawingMode);
	}

	return std::pair<std::vector<Vertex>, std::vector<unsigned int>>{{}, {}};
}
