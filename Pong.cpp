#include "Pong.h"

#include "Engine/Scene/UI/TextUI.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Scene/Level.h"
#include "PlayerPlatform.h"
#include "Ball.h"
#include "Assets/SoundPlayer.h"
#include <Globals.h>

Pong::Pong()
{
	CurEngine = std::make_shared<Engine>();
	CreateLevelAndGameState();
}

void Pong::CreateLevelAndGameState()
{
	PongLevel = std::make_shared<Level>();
    
    // Player pltfroms
    std::shared_ptr<PlayerPlatform> Player1 = std::make_shared<PlayerPlatform>(glm::vec3(44.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 4.f, 1.f), InputKey::UP, InputKey::DOWN);
    std::shared_ptr<PlayerPlatform> Player2 = std::make_shared<PlayerPlatform>(glm::vec3(-44.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 4.f, 1.f), InputKey::W, InputKey::S);
    PongLevel->AddActorOnLevel(Player1);
    PongLevel->AddActorOnLevel(Player2);
    
    // Ball
    std::shared_ptr<Ball> PongBall = std::make_shared<Ball>(1.f, 35.f);
    constexpr auto EndRoundSound = "LostRound.mp3";
    PongBall->BindFunctionOnEndRound(std::bind(& SoundPlayer::PlaySoundFromFile, Globals::GetSoundPlayer(), EndRoundSound));
    constexpr auto PlatformHitSound = "Platform.mp3";
    PongBall->BindFunctionOnPlatformCollision(std::bind(&SoundPlayer::PlaySoundFromFile, Globals::GetSoundPlayer(), PlatformHitSound));
    PongLevel->AddActorOnLevel(PongBall);
    
    // UI
    std::shared_ptr<TextUI> Player1ScoreUI = std::make_shared<TextUI>(std::string("0"), glm::vec3(-30.f, 20.f, -3.f), glm::vec3(0.f), glm::vec3(0.2f));
    std::shared_ptr<TextUI> Player2ScoreUI = std::make_shared<TextUI>(std::string("0"), glm::vec3(20.f, 20.f, -3.f), glm::vec3(0.f), glm::vec3(0.2f));
    Player1ScoreUI->SetColor(glm::vec4(0.7f));
    Player2ScoreUI->SetColor(glm::vec4(0.7f));
    PongLevel->AddUIOnLevel(Player1ScoreUI);
    PongLevel->AddUIOnLevel(Player2ScoreUI);

    // Setting Game state
    GameState = std::make_shared < PongGameState>( 10, Player1ScoreUI, Player2ScoreUI );
    GameState->BindFunctionOnEndPongGame(std::bind(&Pong::EndPongGame, this, std::placeholders::_1));

    PongBall->BindFunctionOnEndRound(std::bind(&PongGameState::AddPointForPlayer, GameState.get(), std::placeholders::_1));
}

void Pong::EndPongGame(PongPlayer Player)
{
    std::stringstream ss;
    ss << "Player " << (Player == PongPlayer::Player_1 ? "1" : "2") << " won";
    std::string Result = ss.str();
    std::shared_ptr<TextUI> PlayerWonUI = std::make_shared<TextUI>(std::string(Result), glm::vec3(-35.f, 0.f, -3.f), glm::vec3(0.f), glm::vec3(0.156f));
    Globals::GetLevel()->AddUIOnLevel(PlayerWonUI);
    constexpr auto WinSound = "Win.mp3";
    Globals::GetSoundPlayer()->PlaySoundFromFile(WinSound);
}


void Pong::RunPong()
{
    CurEngine->Run(PongLevel.get(), GameState.get());
}