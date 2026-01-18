#pragma once
#include "Scene/SceneObject.h"
#include <string>

class TextUI : public SceneObject
{
	TextUI(std::string text, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~TextUI() {};
protected:
	std::string Text;
};