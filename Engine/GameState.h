#pragma once
#include <functional>
#include <vector>

class GameState
{
public:
	GameState() = default;

	void BindFunctionOnEndgame(std::function<void()> func);

protected:
	void EndGame();

private:
	std::vector<std::function<void()>> BindedEndgameFunctions;
};