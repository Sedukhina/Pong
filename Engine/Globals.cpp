#include "Globals.h"

InputManager* Globals::GetInputManager()
{
	return CurrentInputManager;
}

void Globals::SetInputManager(InputManager* NewInputManagerPtr)
{
	CurrentInputManager = NewInputManagerPtr;
}
