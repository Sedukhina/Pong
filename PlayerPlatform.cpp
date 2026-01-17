#include "PlayerPlatform.h"
#include "Engine/Globals.h"
#include "Assets/Asset.h"
#include <glm/gtx/norm.hpp> 
#include "Log.h"

PlayerPlatform::PlayerPlatform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, InputKey UpKey, InputKey DownKey)
	: Actor(position, rotation, scale), MovePlatformUpKey(UpKey), MovePlatformDownKey(DownKey)
{
	// Setting visual representation
	std::string MeshPath = "DonutPainting.fbx";
	Model PlatformModel{ MeshPath, "Donut_tex.jpg" };
	this->AddModel(PlatformModel);

	// Setting collision
	std::array<glm::vec2, 2> AABB = Globals::GetAssetManager()->GetMeshAABB(MeshPath, GetAssetID(MeshPath));
	std::shared_ptr<fcl::Boxf> CollsionBox = std::make_shared<fcl::Boxf>(GetScale().x * (AABB[1].x - AABB[0].x), GetScale().y * (AABB[1].y - AABB[0].y), GetScale().z);
	this->AddCollision(CollsionBox);

	// Binding input
	Globals::GetInputManager()->BindInput(std::bind(&PlayerPlatform::MovePlatform, this, glm::vec3(0.f, 1.f, 0.f), PlatformSpeed), MovePlatformUpKey, InputAction::PRESSED);
	Globals::GetInputManager()->BindInput(std::bind(&PlayerPlatform::MovePlatform, this, glm::vec3(0.f, 1.f, 0.f), -PlatformSpeed), MovePlatformDownKey, InputAction::PRESSED);
}

PlayerPlatform::~PlayerPlatform()
{
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
