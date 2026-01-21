#include "TextUI.h"
#include "Globals.h"
#include "Assets/AssetManager.h"

TextUI::TextUI(std::string text, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: SceneObject(position, rotation, scale)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 4 vertices per character
	glBufferData(GL_ARRAY_BUFFER, MaxUITextSize * 4 * sizeof(TextVertex), nullptr, GL_DYNAMIC_DRAW);
	
	std::vector<unsigned int> Indices;
	Indices.reserve(MaxUITextSize * 6);
	for (unsigned int i = 0; i < MaxUITextSize; i++)
	{
		Indices.push_back(i * 4);
		Indices.push_back(i * 4 + 1);
		Indices.push_back(i * 4 + 2);

		Indices.push_back(i * 4 + 2);
		Indices.push_back(i * 4 + 1);
		Indices.push_back(i * 4 + 3);
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxUITextSize * 6 * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (GLvoid*)offsetof(TextVertex, TextureCoordinateX));

	SetText(text);
}

void TextUI::SetText(std::string text)
{
	if (text.size() > MaxUITextSize)
	{
		return;
	}
	Text = text;
	Globals::GetAssetManager()->GenTextUIVertices(Text, VBO);
}

std::string TextUI::GetText() const
{
	return Text;
}

void TextUI::SetColor(glm::vec4 color)
{
	Color = color;
}

glm::vec4 TextUI::GetColor() const
{
	return Color;
}

unsigned int TextUI::GetIndicesSize() const
{
	return Text.size() * 6;
}

GLuint TextUI::GetVAO() const
{
	return VAO;
}

