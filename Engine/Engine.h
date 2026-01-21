#pragma once
// For smart pointers
#include <memory>
#include "Scene/Level.h"
#include "GameState.h"

class Engine
{
public:
    Engine();

    void Run(Level* CurrentLevel, GameState* CurrentGameState);

    void SetShouldShutdownTrue();

protected:
    std::shared_ptr<class Renderer> CurrentRenderer;

    std::shared_ptr<class InputManager> CurrentInputManager;
    std::shared_ptr<class AssetManager> CurrentAssetManager;
    std::shared_ptr<class SoundPlayer> CurrentSoundPlayer;

    bool ShouldShutdown = false;
};