#pragma once
#include "GameState.h"
#include <array>

class PongGameState : public GameState
{
public:
	PongGameState(int winCondition);
	~PongGameState() {};

	void AddPointForPlayer(int Player);
	void CheckWinCondition();

private:
	int WinCondition;
	std::array<int, 2> Score{ 0, 0 };
};
