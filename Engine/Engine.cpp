#include "Engine.h"
#include "render/Renderer.h"

Engine::Engine()
{
    Render = std::make_unique<Renderer>();
    Render->InitRenderer();
}

Engine::~Engine()
{
}

void Engine::Run()
{
    while (!ShouldShutdown)
    {
        if (Render->Tick())
        {
            SetShouldShutdownTrue();
        }
    }
}

void Engine::SetShouldShutdownTrue()
{
    ShouldShutdown = true;
}
