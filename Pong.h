#pragma once
#include "Engine/Engine.h"
#include "PongGameState.h"

class Pong
{
public:
	Pong();
	void RunPong();

private:
	void CreateLevelAndGameState();
	void EndPongGame(PongPlayer Player);

	std::shared_ptr <Engine> CurEngine;
	std::shared_ptr <Level> PongLevel;
	std::shared_ptr <PongGameState> GameState;
};

