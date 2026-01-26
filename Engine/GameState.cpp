#include "GameState.h"


void GameState::EndGame()
{
	for (std::function<void()> EndGameFunction : BindedEndgameFunctions)
	{
		std::invoke(EndGameFunction);
	}
}

void GameState::BindFunctionOnEndgame(std::function<void()> Function)
{
	BindedEndgameFunctions.push_back(Function);
}
