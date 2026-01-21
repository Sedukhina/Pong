#include "Level.h"

void Level::Tick(float DeltaTime)
{
	for (std::shared_ptr<Actor> ActorOnLevel : ActorsOnLevel)
	{
		ActorOnLevel->Tick(DeltaTime);
	}
}

void Level::AddActorOnLevel(std::shared_ptr<Actor> actor)
{
	ActorsOnLevel.push_back(actor);
}

const std::vector<std::shared_ptr<Actor>>& Level::GetActorsOnLevel() const
{
	return ActorsOnLevel;
}

void Level::AddUIOnLevel(std::shared_ptr<TextUI> actor)
{
	TextUIsOnLevel.push_back(actor);
}

const std::vector<std::shared_ptr<TextUI>>& Level::GetTextUIsOnLevel() const
{
	return TextUIsOnLevel;
}
