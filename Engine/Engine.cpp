#include "Engine.h"
#include "Render/Renderer.h"
#include "Input/InputManager.h"

Engine::Engine()
{
    CurrentRenderer = std::make_unique<Renderer>();
    CurrentRenderer->InitRenderer();
    CurrentInputManager = std::make_shared<InputManager>();
    CurrentInputManager->BindInput(std::bind(&Engine::SetShouldShutdownTrue, this), InputKey::ESC, InputAction::PRESSED);
}

Engine::~Engine()
{
}

void Engine::Run()
{
    while (!ShouldShutdown)
    {
        CurrentRenderer->Tick();
    }
}

void Engine::SetShouldShutdownTrue()
{
    ShouldShutdown = true;
}
