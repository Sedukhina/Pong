#pragma once
// For smart pointers
#include <memory>
#include "Scene/Level.h"

class Engine
{
public:
    Engine();
    ~Engine();

    void Run(Level* CurrentLevel);

    void SetShouldShutdownTrue();

protected:

    std::unique_ptr<class Renderer> CurrentRenderer;

    std::shared_ptr<class InputManager> CurrentInputManager;
    std::shared_ptr<class AssetManager> CurrentAssetManager;

    bool ShouldShutdown = false;
};