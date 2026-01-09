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
    std::unique_ptr<class Renderer> Render;

    bool ShouldShutdown = false;
};