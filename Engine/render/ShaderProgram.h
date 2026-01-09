#pragma once
#include <GL/glew.h>
#include "filesystem"

class ShaderProgram {
private:
	GLuint Program;
public:
	ShaderProgram(std::filesystem::path pathToVertex, std::filesystem::path pathToFragment);

	void Use();
	GLint GetUniformLocation(const char* name);
};