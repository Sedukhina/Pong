#include "Input/InputManager.h"
#include "Log.h"

void InputManager::HandleInput(int key, int action)
{
	if (key < 0 || static_cast<int>(InputKey::LAST_KEY_VALUE) < key || action < 0 || static_cast<int>(InputAction::LAST_ACTION_VALUE) < action)
	{ 
		return;
	}
	if (BindedActions.find(action * static_cast<int>(InputKey::LAST_KEY_VALUE) + key) == BindedActions.end())
	{
		return;
	}
	for (std::function<void()> Action : BindedActions[action * static_cast<int>(InputKey::LAST_KEY_VALUE) + key])
	{
		std::invoke(Action);
	}
}

void InputManager::BindInput(InputActionFunction func, InputKey key, InputAction Action)
{
	BindedActions[static_cast<int>(Action) * static_cast<int>(InputKey::LAST_KEY_VALUE) + static_cast<int>(key)].emplace_back(func);
}

void InputManager::BindInput(InputActionFunction func, int key, int action)
{
	if (key >= 0 && static_cast<int>(InputKey::LAST_KEY_VALUE) >= key && action >= 0 && static_cast<int>(InputAction::LAST_ACTION_VALUE) >= action)
	{
		BindInput(func, static_cast<InputKey>(key), static_cast<InputAction>(action));
	}
}
