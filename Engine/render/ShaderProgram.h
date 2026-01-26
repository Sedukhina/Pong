#pragma once

#include <GL/glew.h>
#include "filesystem"

class ShaderProgram {
private:
	GLuint Program = -1;
public:
	ShaderProgram(std::filesystem::path pathToVertex, std::filesystem::path pathToFragment);

	void Use();
	GLint GetUniformLocation(const char* Name);
};