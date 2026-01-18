#include "TextUI.h"

TextUI::TextUI(std::string text, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: SceneObject(position, rotation, scale), Text(text)
{
}
