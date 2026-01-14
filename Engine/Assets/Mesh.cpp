#include "Assets/Mesh.h"

Mesh::Mesh(GLuint vao, GLuint vbo, GLuint ebo, unsigned int indices) 
	: VAO(vao), VBO(vbo), EBO(ebo), IndicesArraySize(indices)
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

