// Passes input from window to input manager

#pragma once

#include <GLFW/glfw3.h>
#include "Globals.h"
#include "Input/InputManager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Globals::GetInputManager()->HandleInput(key, action);
}