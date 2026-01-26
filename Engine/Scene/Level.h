#pragma once
#include "Scene/Actor.h"
#include "Scene/UI/TextUI.h"
// For smart pointers
#include <memory>
#include <vector>

class Level
{
public:
	void Tick(float DeltaTime);

	void AddActorOnLevel(std::shared_ptr<Actor> NewActor);
	const std::vector<std::shared_ptr<Actor>>& GetActorsOnLevel() const;

	void AddUIOnLevel(std::shared_ptr<TextUI> NewActor);
	const std::vector<std::shared_ptr<TextUI>>& GetTextUIsOnLevel() const;

private:
	std::vector<std::shared_ptr<Actor>> ActorsOnLevel;
	std::vector<std::shared_ptr<TextUI>> TextUIsOnLevel;
};
