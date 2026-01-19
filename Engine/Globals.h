#pragma once

#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Scene/Level.h"
#include "Assets/SoundPlayer.h"
#include "GameState.h"

class Globals
{
public:
	Globals() {};
	~Globals() {};

	static void SetTimeFreezed(bool NewValue);
	static bool GetTimeFreezed();

	static InputManager* GetInputManager();
	static AssetManager* GetAssetManager();
	static Level* GetLevel();
	static SoundPlayer* GetSoundPlayer();

	static void SetInputManager(InputManager* NewInputManagerPtr);
	static void SetAssetManager(AssetManager* NewAssetManagerPtr);
	static void SetLevel(Level* NewLevel);
	static void SetSoundPlayer(SoundPlayer* NewSoundPlayer);

	static const float GetScreenHalfWidth();
	static const float GetScreenHalfHeight();
	static void SetScreenRatio(float ScreenRation);

private:
	inline static bool TimeFreezed = false;

	inline static InputManager* CurrentInputManager = nullptr;
	inline static AssetManager* CurrentAssetManager = nullptr;
	inline static Level* CurrentLevel = nullptr;
	inline static SoundPlayer* CurrentSoundPlayer = nullptr;

	// Used to define min/max screen coordinates
	inline static const float ScreenHalfWidth = 50.f;
	inline static float ScreenHalfHeight = 50.f;
};