#include "Pong.h"

#include "Engine/Scene/UI/TextUI.h"

#include "PlayerPlatform.h"
#include "Ball.h"



Pong::Pong()
{
	CurEngine = std::make_shared<Engine>();
	CreateLevelAndGameState();
}

void Pong::CreateLevelAndGameState()
{
	PongLevel = std::make_shared<Level>();

    std::shared_ptr<PlayerPlatform> Player1 = std::make_shared<PlayerPlatform>(glm::vec3(44.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 4.f, 1.f), InputKey::UP, InputKey::DOWN);
    std::shared_ptr<PlayerPlatform> Player2 = std::make_shared<PlayerPlatform>(glm::vec3(-44.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 4.f, 1.f), InputKey::W, InputKey::S);
    std::shared_ptr<TextUI> Player1ScoreUI = std::make_shared<TextUI>(std::string("0"), glm::vec3(-30.f, 20.f, -3.f), glm::vec3(0.f), glm::vec3(0.2f));
    std::shared_ptr<TextUI> Player2ScoreUI = std::make_shared<TextUI>(std::string("0"), glm::vec3(20.f, 20.f, -3.f), glm::vec3(0.f), glm::vec3(0.2f));
    Player1ScoreUI->SetColor(glm::vec4(0.7f));
    Player2ScoreUI->SetColor(glm::vec4(0.7f));
    PongLevel->AddUIOnLevel(Player1ScoreUI);
    PongLevel->AddUIOnLevel(Player2ScoreUI);
    PongLevel->AddActorOnLevel(Player1);
    PongLevel->AddActorOnLevel(Player2);
    std::shared_ptr<Ball> PongBall = std::make_shared<Ball>(1.f, 20.f);
    PongLevel->AddActorOnLevel(PongBall);
    
    GameState = std::make_shared < PongGameState>( 10, Player1ScoreUI, Player2ScoreUI );
    PongBall->BindFunctionOnEndRound(std::bind(&PongGameState::AddPointForPlayer, GameState.get(), std::placeholders::_1));
}


void Pong::RunPong()
{
    CurEngine->Run(PongLevel.get(), GameState.get());
}