#include "Level.h"
#include "Globals.h"
#include "Render/Renderer.h"

void Level::Tick(float DeltaTime)
{
	for (const auto& ActorOnLevel : ActorsOnLevel)
	{
		ActorOnLevel->Tick(DeltaTime);
	}
}

void Level::AddActorOnLevel(std::shared_ptr<Actor> NewActor)
{
	ActorsOnLevel.push_back(NewActor);
}

const std::vector<std::shared_ptr<Actor>>& Level::GetActorsOnLevel() const
{
	return ActorsOnLevel;
}

void Level::AddUIOnLevel(std::shared_ptr<TextUI> NewActor)
{
	TextUIsOnLevel.push_back(NewActor);
}

const std::vector<std::shared_ptr<TextUI>>& Level::GetTextUIsOnLevel() const
{
	return TextUIsOnLevel;
}
