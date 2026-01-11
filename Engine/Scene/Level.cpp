#include "Level.h"

Level::Level()
{
}

Level::~Level()
{
}

void Level::AddActorOnLevel(std::shared_ptr<Actor> actor)
{
	ActorsOnLevel.push_back(actor);
}

const std::vector<std::shared_ptr<Actor>>& Level::GetActorsOnLevel() const
{
	return ActorsOnLevel;
}
