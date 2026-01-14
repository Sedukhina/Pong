#pragma once
#include <GL/glew.h>

class Mesh
{
public:
	Mesh(GLuint vao, GLuint vbo, GLuint ebo, unsigned int indices);
	~Mesh() {};

	const GLuint GetVAO() const;
	const GLuint GetVBO() const;
	const GLuint GetEBO() const;
	const unsigned int GetIndicesArraySize() const;

private:
	const GLuint VAO;
	const GLuint VBO;
	const GLuint EBO;

	const unsigned int IndicesArraySize;
};
