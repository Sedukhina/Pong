#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"

class Mesh
{
public:
	Mesh(GLuint vao, GLuint vbo, GLuint ebo, unsigned int indices, glm::vec2 Min, glm::vec2 Max);

	const GLuint& GetVAO() const;
	const GLuint& GetVBO() const;
	const GLuint& GetEBO() const;
	const unsigned int GetIndicesArraySize() const;

	const glm::vec2& GetAABBMin() const;
	const glm::vec2& GetAABBMax() const;

	void SetDrawingMode(GLenum NewDrawingMode);
	const GLenum& GetDrawingMode() const;

private:
	GLenum DrawingMode = GL_TRIANGLES;

	glm::vec2 MinCoords;
	glm::vec2 MaxCoords;

	const GLuint VAO;
	const GLuint VBO;
	const GLuint EBO;

	const unsigned int IndicesArraySize;
};
