#include "Globals.h"

#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Scene/Level.h"
#include "Assets/SoundPlayer.h"
#include "GameState.h"

void Globals::SetTimeFreezed(bool NewValue)
{
	TimeFreezed = NewValue;
}

bool Globals::GetTimeFreezed()
{
	return TimeFreezed;
}

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

SoundPlayer* Globals::GetSoundPlayer()
{
	return CurrentSoundPlayer;
}

Renderer* Globals::GetRenderer()
{
	return CurrentRenderer;
}

void Globals::SetInputManager(InputManager* NewInputManagerPtr)
{
	CurrentInputManager = NewInputManagerPtr;
}

void Globals::SetAssetManager(AssetManager* NewAssetManagerPtr)
{
	CurrentAssetManager = NewAssetManagerPtr;
}

void Globals::SetRenderer(Renderer* NewRendererPtr)
{
	CurrentRenderer = NewRendererPtr;
}

void Globals::SetLevel(Level* NewLevel)
{
	CurrentLevel = NewLevel;
}

void Globals::SetSoundPlayer(SoundPlayer* NewSoundPlayer)
{
	CurrentSoundPlayer = NewSoundPlayer;
}

const float Globals::GetScreenHalfWidth()
{
	return ScreenHalfWidth;
}

const float Globals::GetScreenHalfHeight()
{
	return ScreenHalfHeight;
}

void Globals::SetScreenRatio(float ScreenRation)
{
	ScreenHalfHeight /= ScreenRation;
}

