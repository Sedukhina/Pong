#pragma once

// For smart pointers
#include <memory>

#include "Input/InputManager.h"

class Globals
{
public:
	Globals() {};
	~Globals() {};

	static InputManager* GetInputManager();

	static void SetInputManager(InputManager* NewInputManagerPtr);

private:
	inline static InputManager* CurrentInputManager = nullptr;
};