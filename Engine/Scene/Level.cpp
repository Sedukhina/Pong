#include "Level.h"
#include "Globals.h"
#include "Render/Renderer.h"

void Level::Tick(float DeltaTime)
{
	for (const auto& ActorOnLevel : ActorsOnLevel)
	{
		ActorOnLevel->Tick(DeltaTime);
	}
	Renderer* CurrentRenderer = Globals::GetRenderer();
	CurrentRenderer->RenderModels(GetActorsOnLevel());
	CurrentRenderer->RenderTextUIs(GetTextUIsOnLevel());
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
