#pragma once
#include "Scene/Actor.h"
// For smart pointers
#include <memory>
#include <vector>

class Level
{
public:
	Level();
	~Level();

	void AddActorOnLevel(std::shared_ptr<Actor> actor);
	const std::vector<std::shared_ptr<Actor>>& GetActorsOnLevel() const;

private:
	std::vector<std::shared_ptr<Actor>> ActorsOnLevel;
};
