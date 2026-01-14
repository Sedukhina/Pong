#pragma once

#include "Input/InputManager.h"
#include "Assets/AssetManager.h"

// Used to define min/max screen coordinates
const float ScreenHalfHeight = 50.f;
const float ScreenHalfWidth = 50.f;

class Globals
{
public:
	Globals() {};
	~Globals() {};

	static InputManager* GetInputManager();
	static AssetManager* GetAssetManager();

	static void SetInputManager(InputManager* NewInputManagerPtr);
	static void SetAssetManager(AssetManager* NewAssetManagerPtr);

private:
	inline static InputManager* CurrentInputManager = nullptr;
	inline static AssetManager* CurrentAssetManager = nullptr;
};