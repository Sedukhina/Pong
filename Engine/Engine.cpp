#include "Engine.h"
#include "Scene/Level.h"
#include "GameState.h"
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
    CurrentRenderer = std::make_shared<Renderer>();
    CurrentInputManager = std::make_shared<InputManager>();
    CurrentInputManager->BindInput(std::bind(&Engine::SetShouldShutdownTrue, this), InputKey::ESC, InputAction::PRESSED);
    Globals::SetInputManager(CurrentInputManager.get());
    CurrentSoundPlayer = std::make_shared<SoundPlayer>();
    Globals::SetSoundPlayer(CurrentSoundPlayer.get());
}


void Engine::Run(Level* CurrentLevel, GameState* CurrentGameState)
{
    Globals::SetTimeFreezed(false);
    std::chrono::steady_clock::time_point LastFrame = std::chrono::steady_clock::now();
    float DeltaTime = 0.f;
    Globals::SetLevel(CurrentLevel);
    CurrentGameState->BindFunctionOnEndgame(std::bind(&Globals::SetTimeFreezed, true));

    while (!ShouldShutdown && !CurrentRenderer->GetWindowShouldCLose())
    {
        // Polling events
        CurrentRenderer->PollWindowEvents();

        // Updating delta time
        if (!Globals::GetTimeFreezed())
        {
            DeltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - LastFrame).count();
            LastFrame = std::chrono::steady_clock::now();
        }
        else
        {
            DeltaTime = 0;
        }

        // Game logic
        CurrentLevel->Tick(DeltaTime);

        // Render
        CurrentRenderer->BeginFrame();
        CurrentRenderer->RenderModels(CurrentLevel->GetActorsOnLevel());
        CurrentRenderer->RenderTextUIs(CurrentLevel->GetTextUIsOnLevel());
        CurrentRenderer->EndFrame();
    }
    Globals::SetLevel(nullptr);
}

void Engine::SetShouldShutdownTrue()
{
    ShouldShutdown = true;
}

