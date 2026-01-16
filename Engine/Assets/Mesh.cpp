#include "Assets/Mesh.h"

Mesh::Mesh(GLuint vao, GLuint vbo, GLuint ebo, unsigned int indices, glm::vec2 Min, glm::vec2 Max)
	: VAO(vao), VBO(vbo), EBO(ebo), IndicesArraySize(indices), MinCoords(Min), MaxCoords(Max)
{
}

const GLuint Mesh::GetVAO() const
{
	return VAO;
}

const GLuint Mesh::GetVBO() const
{
	return VBO;
}

const GLuint Mesh::GetEBO() const
{
	return EBO;
}

const unsigned int Mesh::GetIndicesArraySize() const
{
	return IndicesArraySize;
}

glm::vec2 Mesh::GetAABBMin() const
{
	return MinCoords;
}

glm::vec2 Mesh::GetAABBMax() const
{
	return MaxCoords;
}

void Mesh::SetDrawingMode(GLenum NewDrawingMode)
{
	DrawingMode = NewDrawingMode;
}

GLenum Mesh::GetDrawingMode() const
{
	return DrawingMode;
}


