#include "Globals.h"

InputManager* Globals::GetInputManager()
{
	return CurrentInputManager;
}

AssetManager* Globals::GetAssetManager()
{
	return CurrentAssetManager;
}

Level* Globals::GetLevel()
{
	return CurrentLevel;
}

void Globals::SetInputManager(InputManager* NewInputManagerPtr)
{
	CurrentInputManager = NewInputManagerPtr;
}

void Globals::SetAssetManager(AssetManager* NewAssetManagerPtr)
{
	CurrentAssetManager = NewAssetManagerPtr;
}

void Globals::SetLevel(Level* NewLevel)
{
	CurrentLevel = NewLevel;
}

