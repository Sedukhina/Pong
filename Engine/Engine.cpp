#include "Engine.h"
#include "Render/Renderer.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Globals.h"
#include "Log.h"
#include "Assets/SoundPlayer.h"
#include <time.h>

Engine::Engine()
{
    CurrentAssetManager = std::make_shared<AssetManager>();
    Globals::SetAssetManager(CurrentAssetManager.get());
    CurrentRenderer = std::make_unique<Renderer>();
    CurrentRenderer->InitRenderer();
    CurrentInputManager = std::make_shared<InputManager>();
    CurrentInputManager->BindInput(std::bind(&Engine::SetShouldShutdownTrue, this), InputKey::ESC, InputAction::PRESSED);
    Globals::SetInputManager(CurrentInputManager.get());
    CurrentSoundPlayer = std::make_shared<SoundPlayer>();
    Globals::SetSoundPlayer(CurrentSoundPlayer.get());
}

Engine::~Engine()
{

}

void Engine::Run(Level* CurrentLevel, GameState* CurrentGameState)
{
    Globals::SetTimeFreezed(false);
    clock_t LastFrame = clock();
    clock_t DeltaTime = 0;
    Globals::SetLevel(CurrentLevel);
    CurrentGameState->BindFunctionOnEndgame(std::bind(&Globals::SetTimeFreezed, true));
    while (!ShouldShutdown && !CurrentRenderer->GetWindowShouldCLose())
    {
        if (!Globals::GetTimeFreezed())
        {
            DeltaTime = clock() - LastFrame + 1;
        }
        else
        {
            DeltaTime = 0;
        }
        CurrentLevel->Tick(DeltaTime);
        CurrentRenderer->Tick(DeltaTime);
        LastFrame = clock();
    }
    Globals::SetLevel(nullptr);
}

void Engine::SetShouldShutdownTrue()
{
    ShouldShutdown = true;
}

