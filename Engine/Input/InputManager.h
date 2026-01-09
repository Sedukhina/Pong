#pragma once

#include "Input/InputTypes.h"
#include <map>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void HandleInput(int Key, int Action);

	void BindInput(InputActionFunction func, InputKey key, InputAction action);
	void BindInput(InputActionFunction func, int key, int action);

private:
	std::map<int, std::vector<InputActionFunction>> BindedActions;
};

