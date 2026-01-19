#include "PongGameState.h"
#include "Log.h"

PongGameState::PongGameState(int winCondition, std::weak_ptr<TextUI> Player1ScoreUI, std::weak_ptr<TextUI> Player2ScoreUI)
	: WinCondition(winCondition), PlayerScoreUIs { Player1ScoreUI, Player2ScoreUI }
{
}

void PongGameState::AddPointForPlayer(int Player)
{
	if (Player != 1 && Player != 0)
	{
		return;
	}
	Score[Player] += 1;
	PlayerScoreUIs[Player].lock()->SetText(std::to_string(Score[Player]));
	CheckWinCondition();
}

void PongGameState::CheckWinCondition()
{
	if (Score[0] >= WinCondition || Score[1] >= WinCondition)
	{
		EndGame();
	}
	char buffer[300];
	sprintf(buffer, "%d %d", Score[0], Score[1]);
	LOG_INFO(buffer);
}
