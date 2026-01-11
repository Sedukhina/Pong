#include "Globals.h"

InputManager* Globals::GetInputManager()
{
	return CurrentInputManager;
}

AssetManager* Globals::GetAssetManager()
{
	return CurrentAssetManager;
}

void Globals::SetInputManager(InputManager* NewInputManagerPtr)
{
	CurrentInputManager = NewInputManagerPtr;
}

void Globals::SetAssetManager(AssetManager* NewAssetManagerPtr)
{
	CurrentAssetManager = NewAssetManagerPtr;
}
