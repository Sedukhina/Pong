#include "PongGameState.h"
#include "Globals.h"
#include "Scene/Level.h"
#include "Assets/SoundPlayer.h"

PongGameState::PongGameState(int winCondition, std::weak_ptr<TextUI> Player1ScoreUI, std::weak_ptr<TextUI> Player2ScoreUI)
	: WinCondition(winCondition), PlayerScoreUIs { Player1ScoreUI, Player2ScoreUI }
{
}

void PongGameState::AddPointForPlayer(PongPlayer Player)
{
	int PlayerNum = static_cast<int>(Player);
	Score[PlayerNum] += 1;
	std::string PlayerScore = std::to_string(Score[PlayerNum]);
	PlayerScoreUIs[PlayerNum].lock()->SetText(PlayerScore);
	CheckWinCondition();
}

void PongGameState::CheckWinCondition()
{
	if (Score[0] >= WinCondition || Score[1] >= WinCondition)
	{
		Endgame(Score[0] >= WinCondition ? PongPlayer::Player_1 : PongPlayer::Player_2);
	}
}

void PongGameState::BindFunctionOnEndPongGame(std::function<void(PongPlayer)> Func)
{
	OnPongGameEnd.push_back(Func);
}

void PongGameState::Endgame(PongPlayer Player)
{
	GameState::EndGame();
	for (std::function<void(PongPlayer)> EndGameFunction : OnPongGameEnd)
	{
		std::invoke(EndGameFunction, Player);
	}
}
