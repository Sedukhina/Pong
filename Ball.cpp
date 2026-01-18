#include "Ball.h"
#include "Engine/Assets/Generated.h"
#include "Engine/Log.h"
#include "Globals.h"
#include "Assets/Asset.h"
#include <random>

Ball::Ball(float radius, float speed)
	: Speed(speed), Radius(radius)
{
	std::filesystem::path MeshPath = GeneratePathForCircle(radius, Segments);
	Model PlatformModel{ MeshPath, "ink.jpg" };

	// Setting collision
	std::array<glm::vec2, 2> AABB = Globals::GetAssetManager()->GetMeshAABB(MeshPath, GetAssetID(MeshPath));
	std::shared_ptr<fcl::Spheref> CollsionSphere = std::make_shared<fcl::Spheref>(radius);
	this->AddCollision(CollsionSphere);

	this->AddModel(PlatformModel);

	StartRound();
}

void Ball::BindFunctionOnEndRound(std::function<void(int)> func)
{
	OnRoundEndBindedFunctions.push_back(func);
}

void Ball::Tick(float DeltaTime)
{ 
	Level* CurrentLevel = Globals::GetLevel();
	if (!CurrentLevel)
	{
		return;
	}
	const std::vector<std::shared_ptr<Actor>> ActorsOnLevel = CurrentLevel->GetActorsOnLevel();
	MoveBall(ActorsOnLevel, Speed * DeltaTime);
}

void Ball::StartRound()
{
	SetPosition(glm::vec3(0.f));

	static std::random_device RandomDevice;
	static std::mt19937 Generator(RandomDevice());
	static std::uniform_real_distribution<float> Distribution(-1.0f, 1.0f);
	Direction = glm::normalize(glm::vec3(Distribution(Generator), Distribution(Generator), 0.f));
}

void Ball::EndRound(bool Player)
{
	for (std::function<void(int)> EndGameFunction : OnRoundEndBindedFunctions)
	{
		std::invoke(EndGameFunction, Player);
	}
	StartRound();
}

void Ball::MoveBall(const std::vector<std::shared_ptr<Actor>> &ActorsOnLevel, float Step)
{
	glm::vec3 NewPosition = GetPosition();

	float ScreenHalfWidth = Globals::GetScreenHalfWidth();
	float ScreenHalfHeight = Globals::GetScreenHalfHeight();

	while (Step != 0)
	{
		float ActorDistance = -1.f;
		Actor* OtherActor = nullptr;
		CheckCollisionWithActors(ActorsOnLevel, Step, &ActorDistance, OtherActor);
		float WallDistance = -1;
		std::array<bool, 2> Directions = CheckCollisionWithWalls(Step, &WallDistance);

		if (WallDistance == -1.f && ActorDistance == -1.f)
		{
			NewPosition += Direction * Step;
			Step = 0.f;
		}
		else if (WallDistance != -1.f && ((WallDistance < ActorDistance) != (ActorDistance == -1.f)))
		{
			NewPosition += Direction * WallDistance;
			Step -= WallDistance;
			if (Directions[0]) 
			{ 
				EndRound(Direction.y > 0); 
				return;
			};
			if (Directions[1]) { Direction.y = -Direction.y; };
		}
	}

	SetPosition(NewPosition);
}

std::array<bool, 2> Ball::CheckCollisionWithWalls(float Step, float* Distance)
{
	std::array<bool, 2> Directions{false, false};
	float ScreenHalfWidth = Globals::GetScreenHalfWidth();
	float ScreenHalfHeight = Globals::GetScreenHalfHeight();
	glm::vec3 NewPosition = GetPosition() + Direction * Step;

	if (Direction.x > 0)
	{
		if (NewPosition.x + Radius > ScreenHalfWidth)
		{
			*Distance = ScreenHalfWidth - GetPosition().x - Radius;
			Directions[0] = true;
		}
	}
	else 
	{
		if (NewPosition.x - Radius < -ScreenHalfWidth)
		{
			*Distance = GetPosition().x + ScreenHalfWidth - Radius;
			Directions[0] = true;
		}
	}

	float Dist = -1;
	if (Direction.y > 0)
	{
		if (NewPosition.y + Radius > ScreenHalfHeight)
		{
			Dist = ScreenHalfHeight - GetPosition().y - Radius;
		}
	}
	else 
	{
		if (NewPosition.y - Radius < -ScreenHalfHeight)
		{
			Dist = GetPosition().y + ScreenHalfHeight - Radius;
		}
	}

	if (Dist == -1.f)
	{
		return Directions;
	}

	if (Dist < *Distance || *Distance == -1.f)
	{
		*Distance = Dist;
		Directions[0] = false;
		Directions[1] = true;
	}
	else if (Dist == *Distance)
	{
		Directions[1] = true;
	}
	return Directions;
}

void Ball::CheckCollisionWithActors(const std::vector<std::shared_ptr<Actor>>& ActorsOnLevel, float Step, float* Distance, Actor* OtherActor)
{
	for (std::shared_ptr<Actor> ActorOnLevel : ActorsOnLevel)
	{
		if (ActorOnLevel.get() == this)
		{
			continue;
		}
		std::shared_ptr<fcl::CollisionObjectf> OtherActorCollision = ActorOnLevel->GetActorCollision();
		if (!OtherActorCollision)
		{
			continue;
		}
		fcl::ContinuousCollisionRequestf Request;
		fcl::ContinuousCollisionResultf Result;

		const fcl::Vector3f StartTranslation = GetActorCollision()->getTransform().translation();
		fcl::Vector3f EndTranslation = StartTranslation + fcl::Vector3f(Direction.x * Step, Direction.y * Step, 0);
		fcl::Quaternionf EndRotation = GetActorCollision()->getQuatRotation();
		fcl::Transform3f EndTransform = GetActorCollision()->getTransform();

		fcl::Transform3f OtherActorTransform = OtherActorCollision->getTransform();
		fcl::continuousCollide(GetActorCollision().get(),
		  EndTransform,
		  OtherActorCollision.get(),
		  OtherActorTransform,
		  Request, Result);
	}
	return;
}
