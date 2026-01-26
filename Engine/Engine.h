#pragma once
// For smart pointers
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Engine
{
public:
    Engine();
    ~Engine();

    void Run(class Level* CurrentLevel, class GameState* CurrentGameState);

    void SetShouldShutdownTrue();

protected:
    void CreateEngineWindow();
    GLFWwindow* Window = nullptr;

    std::shared_ptr<class Renderer> CurrentRenderer;
    std::shared_ptr<class InputManager> CurrentInputManager;
    std::shared_ptr<class AssetManager> CurrentAssetManager;
    std::shared_ptr<class SoundPlayer> CurrentSoundPlayer;

    bool ShouldShutdown = false;
};