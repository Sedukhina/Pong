#pragma once
// For smart pointers
#include <memory>

class Engine
{
public:
    Engine();

    void Run(class Level* CurrentLevel, class GameState* CurrentGameState);

    void SetShouldShutdownTrue();

protected:
    std::shared_ptr<class Renderer> CurrentRenderer;

    std::shared_ptr<class InputManager> CurrentInputManager;
    std::shared_ptr<class AssetManager> CurrentAssetManager;
    std::shared_ptr<class SoundPlayer> CurrentSoundPlayer;

    bool ShouldShutdown = false;
};