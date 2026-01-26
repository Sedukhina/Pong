#include "PlayerPlatform.h"
#include "Engine/Globals.h"
#include "Assets/Asset.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include <glm/gtx/norm.hpp> 

PlayerPlatform::PlayerPlatform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, InputKey UpKey, InputKey DownKey, std::shared_ptr<Model> PlatformModel)
	: Actor(position, rotation, scale), MovePlatformUpKey(UpKey), MovePlatformDownKey(DownKey)
{
	this->AddModel(PlatformModel);

	// Setting collision
	std::array<glm::vec2, 2> AABB = Globals::GetAssetManager()->GetMeshAABB(PlatformModel->GetMeshPath(), GetAssetID(PlatformModel->GetMeshPath()));
	std::shared_ptr<fcl::Boxf> CollsionBox = std::make_shared<fcl::Boxf>(GetScale().x * (AABB[1].x - AABB[0].x), GetScale().y * (AABB[1].y - AABB[0].y), GetScale().z);
	this->AddCollision(CollsionBox);

	// Binding input
	Globals::GetInputManager()->BindInput(std::bind(&PlayerPlatform::RequestPlatformMovement, this, glm::vec3(0.f, 1.f, 0.f)), MovePlatformUpKey, InputAction::PRESSED);
	Globals::GetInputManager()->BindInput(std::bind(&PlayerPlatform::RequestPlatformMovement, this, glm::vec3(0.f, -1.f, 0.f)), MovePlatformDownKey, InputAction::PRESSED);
	Globals::GetInputManager()->BindInput(std::bind(&PlayerPlatform::RequestPlatformMovement, this, glm::vec3(0.f, 1.f, 0.f)), MovePlatformUpKey, InputAction::REPEATED);
	Globals::GetInputManager()->BindInput(std::bind(&PlayerPlatform::RequestPlatformMovement, this, glm::vec3(0.f, -1.f, 0.f)), MovePlatformDownKey, InputAction::REPEATED);
}

void PlayerPlatform::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);
	if (RequestedDirection != glm::vec3(0.f))
	{
		MovePlatform(glm::normalize(RequestedDirection), PlatformSpeed * DeltaTime);
		RequestedDirection = glm::vec3(0.f);
	}
}

void PlayerPlatform::RequestPlatformMovement(glm::vec3 direction)
{
	RequestedDirection += direction;
}

void PlayerPlatform::MovePlatform(glm::vec3 direction, float step)
{
	if (glm::length2(direction) < 1e-4f)
		return;
	direction = glm::normalize(direction);
	glm::vec3 NewPosition = GetPosition() + direction * step;

	glm::vec3 Diff = NewPosition - GetPosition();
	fcl::AABBf ActorColisionAABB = GetActorCollision()->getAABB();

	float ScreenHalfWidth = Globals::GetScreenHalfWidth();
	float ScreenHalfHeight = Globals::GetScreenHalfHeight();

	if (ActorColisionAABB.min_.x() + Diff.x <= -ScreenHalfWidth)
	{
		NewPosition.x = -ScreenHalfWidth + (GetPosition().x - ActorColisionAABB.min_.x());
	}
	else if (ActorColisionAABB.max_.x() + Diff.x >= ScreenHalfWidth)
	{
		NewPosition.x = ScreenHalfWidth - (ActorColisionAABB.max_.x() - GetPosition().x);
	}
	if (ActorColisionAABB.min_.y() + Diff.y <= -ScreenHalfHeight)
	{
		NewPosition.y = -ScreenHalfHeight + (GetPosition().y - ActorColisionAABB.min_.y());
	}
	else if (ActorColisionAABB.max_.y() + Diff.y >= ScreenHalfHeight)
	{
		NewPosition.y = ScreenHalfHeight - (ActorColisionAABB.max_.y() - GetPosition().y);
	}

	SetPosition(NewPosition);
}
