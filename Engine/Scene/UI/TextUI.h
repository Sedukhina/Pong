#pragma once
#include "Scene/SceneObject.h"
#include <string>
#include <GL/glew.h>

const int MaxUITextSize = 300;

class TextUI : public SceneObject
{
public:
	TextUI(std::string text, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void SetText(std::string text);
	std::string GetText() const;

	void SetColor(glm::vec4 color);
	glm::vec4 GetColor() const;

	GLuint GetVAO() const;
	unsigned int GetIndicesSize() const;

protected:
	std::string Text;
	glm::vec4 Color = glm::vec4(1.f);

	GLuint VAO = -1;
	GLuint VBO = -1;
	GLuint EBO = 1;
};