#include "Engine.h"
#include "Render/Renderer.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Globals.h"
#include "Log.h"

Engine::Engine()
{
    CurrentRenderer = std::make_unique<Renderer>();
    CurrentRenderer->InitRenderer();
    CurrentInputManager = std::make_shared<InputManager>();
    CurrentInputManager->BindInput(std::bind(&Engine::SetShouldShutdownTrue, this), InputKey::ESC, InputAction::PRESSED);
    Globals::SetInputManager(CurrentInputManager.get());
    CurrentAssetManager = std::make_shared<AssetManager>();
    Globals::SetAssetManager(CurrentAssetManager.get());
}

Engine::~Engine()
{

}

void Engine::Run(Level* CurrentLevel)
{
    Globals::SetLevel(CurrentLevel);
    while (!ShouldShutdown)
    {
        CurrentRenderer->Tick();
    }
    Globals::SetLevel(nullptr);
}

void Engine::SetShouldShutdownTrue()
{
    ShouldShutdown = true;
}
