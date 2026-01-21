#include "Ball.h"
// Ball mesh generation
#include "Engine/Assets/Generated.h"
#include "Globals.h"
#include "Assets/Asset.h"
#include "Assets/AssetManager.h"
#include "Scene/Level.h"
#include "Assets/SoundPlayer.h"
#include <random>

Ball::Ball(float radius, float speed)
	: Speed(speed), Radius(radius)
{
	std::filesystem::path MeshPath = GeneratePathForCircle(radius, Segments);
	std::shared_ptr<Model> BallModel = std::make_shared<Model>(MeshPath, TexturePath);
	// Setting collision
	std::array<glm::vec2, 2> AABB = Globals::GetAssetManager()->GetMeshAABB(MeshPath, GetAssetID(MeshPath));
	std::shared_ptr<fcl::Spheref> CollsionSphere = std::make_shared<fcl::Spheref>(radius);
	this->AddCollision(CollsionSphere);

	this->AddModel(BallModel);

	StartRound();
}

void Ball::BindFunctionOnEndRound(std::function<void(PongPlayer)> func)
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
	static std::uniform_real_distribution<float> Distribution(-0.1f, 1.f);
	Direction = glm::normalize(glm::vec2(Distribution(Generator), Distribution(Generator)));
}

void Ball::EndRound(PongPlayer Player)
{
	Globals::GetSoundPlayer()->PlaySoundFromFile(EndRoundSound);
	for (std::function<void(PongPlayer)> EndGameFunction : OnRoundEndBindedFunctions)
	{
		std::invoke(EndGameFunction, Player);
	}
	StartRound();
}

void Ball::MoveBall(const std::vector<std::shared_ptr<Actor>> &ActorsOnLevel, float Step)
{
	glm::vec3 NewPosition = GetPosition();
	while (Step > 0.005f)
	{
		float ActorDistance = -1.f;
		Actor* OtherActor = nullptr;
		glm::vec2 ContactNormal = CheckCollisionWithActors(ActorsOnLevel, Step, &ActorDistance);
		float WallDistance = -1;
		std::array<bool, 2> Directions = CheckCollisionWithWalls(Step, &WallDistance);

		if (ActorDistance == 0.f)
		{
			Direction = -Direction;
			NewPosition += glm::vec3(Direction, 0.f) * (Radius + 3.f);
			SetPosition(NewPosition);
			Globals::GetSoundPlayer()->PlaySoundFromFile(PlatformSound);
			return;
		}
		// No collision
		else if (WallDistance == -1.f && ActorDistance == -1.f)
		{
			NewPosition += glm::vec3(Direction, 0.f) * Step;
			Step = 0.f;
		}
		// Wall collision
		else if (WallDistance != -1.f && ((WallDistance < ActorDistance) != (ActorDistance == -1.f)))
		{
			NewPosition += glm::vec3(Direction, 0.f) * WallDistance;
			Step -= WallDistance;
			if (Directions[0]) 
			{ 
				EndRound(Direction.x < 0 ? PongPlayer::Player_1 : PongPlayer::Player_2);
				return;
			};
			if (Directions[1]) { Direction.y = -Direction.y; };
			SetPosition(NewPosition);
		}
		// Other collision
		else
		{
			NewPosition += glm::vec3(Direction, 0.f) * ActorDistance;
			Direction = glm::normalize(Direction - 2.0f * glm::dot(Direction, ContactNormal) * ContactNormal);
			Direction = glm::normalize(Direction);
			// Offset from surface
			NewPosition += glm::vec3(Direction, 0.f) * .5f;
			Step -= ActorDistance;
			SetPosition(NewPosition);
			Globals::GetSoundPlayer()->PlaySoundFromFile(PlatformSound);
		}
	}

	SetPosition(NewPosition);
}

std::array<bool, 2> Ball::CheckCollisionWithWalls(float Step, float* Distance)
{
	std::array<bool, 2> Directions{false, false};
	float ScreenHalfWidth = Globals::GetScreenHalfWidth();
	float ScreenHalfHeight = Globals::GetScreenHalfHeight();
	glm::vec3 NewPosition = GetPosition() + glm::vec3(Direction, 0.f) * Step;

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

glm::vec2 Ball::CheckCollisionWithActors(const std::vector<std::shared_ptr<Actor>>& ActorsOnLevel, float Step, float* Distance)
{
	glm::vec2 ContactNormal = glm::vec2(0.f);
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
		
		float RemainedStep = Step;
		fcl::AABBf OtherActorColisionAABB = OtherActorCollision->getAABB();
		glm::vec2 NewLoc = GetPosition();
		while (RemainedStep > 0.005f)
		{
			glm::vec2 NearestPoint = glm::vec2(
				glm::clamp(NewLoc.x, OtherActorColisionAABB.min_.x(), OtherActorColisionAABB.max_.x()),
				glm::clamp(NewLoc.y, OtherActorColisionAABB.min_.y(), OtherActorColisionAABB.max_.y()));
			// If inside other object
			if (NearestPoint == NewLoc)
			{
				*Distance = 0.f;
				return glm::vec2();
			}
			float DistBetweenCenterAndAABB = glm::distance(NearestPoint, NewLoc);
			// If distance is bigger then step
			if (DistBetweenCenterAndAABB > RemainedStep + Radius)
			{
				RemainedStep = 0;
			}
			// If contact with surface
			else if (DistBetweenCenterAndAABB <= Radius)
			{
				if (*Distance > Step - RemainedStep || *Distance == -1)
				{
					*Distance = Step - RemainedStep;
					RemainedStep = 0;
					ContactNormal = glm::normalize(NewLoc - NearestPoint);
				}
			}
			// Else: walk maximum safe distance and try all conditions again
			else
			{
				NewLoc += Direction * (DistBetweenCenterAndAABB - Radius);
				RemainedStep -= (DistBetweenCenterAndAABB - Radius);
			}
		}
	}
	return ContactNormal;
}
