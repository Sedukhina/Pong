#include "PongGameState.h"
#include "Log.h"
#include "Globals.h"

PongGameState::PongGameState(int winCondition, std::weak_ptr<TextUI> Player1ScoreUI, std::weak_ptr<TextUI> Player2ScoreUI)
	: WinCondition(winCondition), PlayerScoreUIs { Player1ScoreUI, Player2ScoreUI }
{
}

void PongGameState::AddPointForPlayer(PongPlayer Player)
{
	int PlayerNum = static_cast<int>(Player) - 1;
	Score[PlayerNum] += 1;
	std::string PlayerScore = std::to_string(Score[PlayerNum]);
	PlayerScoreUIs[PlayerNum].lock()->SetText(PlayerScore);
	CheckWinCondition();
}

void PongGameState::CheckWinCondition()
{
	if (Score[0] >= WinCondition || Score[1] >= WinCondition)
	{
		std::stringstream ss;
		ss << "Player " << (Score[0] >= WinCondition ? "1" : "2") << " won";
		std::string Result = ss.str();
		std::shared_ptr<TextUI> PlayerWonUI = std::make_shared<TextUI>(std::string(Result), glm::vec3(-35.f, 0.f, -3.f), glm::vec3(0.f), glm::vec3(0.156f));
		Globals::GetLevel()->AddUIOnLevel(PlayerWonUI);
		Globals::GetSoundPlayer()->PlaySoundFromFile(WinSound);
		EndGame();
	}
}
