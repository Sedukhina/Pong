#pragma once
#include "GameState.h"
#include "Scene/UI/TextUI.h"
#include <array>
#include <memory>
#include <filesystem>
#include "PongPlayer.h"

class PongGameState : public GameState
{
public:
	PongGameState(int winCondition, std::weak_ptr<TextUI> Player1ScoreUI, std::weak_ptr<TextUI> Player2ScoreUI);
	~PongGameState() {};

	void AddPointForPlayer(PongPlayer Player);
	void CheckWinCondition();

private:
	int WinCondition;
	std::array<int, 2> Score{ 0, 0 };
	std::array<std::weak_ptr<TextUI>, 2> PlayerScoreUIs;

	std::filesystem::path WinSound = "Win.mp3";
};
