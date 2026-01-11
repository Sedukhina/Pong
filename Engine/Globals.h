#pragma once

#include "Input/InputManager.h"
#include "Assets/AssetManager.h"

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