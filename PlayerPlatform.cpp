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
	std::array<glm::vec2, 2> AABB = Globals::GetAssetManager()->GetMeshAABB(GetAssetID(MeshPath));

	char buffer[300];
	//sprintf(buffer, "AABB min x is %f\nAABB min y is %f", AABB[0].x, AABB[0].y);
	sprintf(buffer, "AABB min x is %f\nAABB min y is %f", 5.f, 5.f);
	LOG_DEBUG(buffer);
	std::shared_ptr<fcl::Boxf> CollsionBox = std::make_shared<fcl::Boxf>(AABB[1].x - AABB[0].x, AABB[1].y - AABB[0].y, 0.f);
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

	SetPosition(NewPosition);
}
