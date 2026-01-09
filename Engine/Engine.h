#pragma once
// For smart pointers
#include <memory>

class Engine
{
public:
    Engine();
    ~Engine();

    void Run();

    void SetShouldShutdownTrue();

protected:

    std::unique_ptr<class Renderer> CurrentRenderer;

    std::shared_ptr<class InputManager> CurrentInputManager;

    bool ShouldShutdown = false;
};