#pragma once
#include <functional>
#include <vector>

class GameState
{
public:
	GameState() = default;

	void BindFunctionOnEndgame(std::function<void()> Function);

protected:
	void EndGame();

private:
	std::vector<std::function<void()>> BindedEndgameFunctions;
};