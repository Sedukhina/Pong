#pragma once

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

struct TextVertex
{
	float PositionX;
	float PositionY;

	float TextureCoordinateX;
	float TextureCoordinateY;
};