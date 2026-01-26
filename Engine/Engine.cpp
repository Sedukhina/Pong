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
#include "Input/InputCallback.h"

Engine::Engine()
{
    CurrentAssetManager = std::make_shared<AssetManager>();
    CreateEngineWindow();
    Globals::SetAssetManager(CurrentAssetManager.get());
    CurrentRenderer = std::make_shared<Renderer>(Window);
    CurrentInputManager = std::make_shared<InputManager>();
    CurrentInputManager->BindInput(std::bind(&Engine::SetShouldShutdownTrue, this), InputKey::ESC, InputAction::PRESSED);
    Globals::SetInputManager(CurrentInputManager.get());
    CurrentSoundPlayer = std::make_shared<SoundPlayer>();
    Globals::SetSoundPlayer(CurrentSoundPlayer.get());
}

Engine::~Engine()
{
    glfwSetWindowShouldClose(Window, GL_TRUE);
    glfwDestroyWindow(Window);
    glfwTerminate();
}


void Engine::Run(Level* CurrentLevel, GameState* CurrentGameState)
{
    Globals::SetTimeFreezed(false);
    std::chrono::steady_clock::time_point LastFrame = std::chrono::steady_clock::now();
    float DeltaTime = 0.f;
    Globals::SetLevel(CurrentLevel);
    CurrentGameState->BindFunctionOnEndgame(std::bind(&Globals::SetTimeFreezed, true));

    while (!ShouldShutdown && !glfwWindowShouldClose(Window))
    {
        // Polling events
        glfwPollEvents();

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

void Engine::CreateEngineWindow()
{
    // Window setting section
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    Window = glfwCreateWindow(static_cast<int>(mode->width * 0.8f), static_cast<int>(mode->height * 0.8f), "Pong", nullptr, nullptr);
    Globals::SetScreenRatio(static_cast<float>(mode->width) / static_cast<float>(mode->height));

    if (Window == nullptr)
    {
        LOG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(Window);

    glfwSetKeyCallback(Window, key_callback);
}

