#include "PlayerPlatform.h"
#include "Engine/Globals.h"

PlayerPlatform::PlayerPlatform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, InputKey UpKey, InputKey DownKey)
	: Actor(position, rotation, scale), MovePlatformUpKey(UpKey), MovePlatformDownKey(DownKey)
{
	Model PlatformModel{ "DonutPainting.fbx", "Donut_tex.jpg" };
	this->AddModel(PlatformModel);
	Globals::GetInputManager()->BindInput(std::bind(&Actor::Move, this, glm::vec3(0.f, 1.f, 0.f), 1.f), MovePlatformUpKey, InputAction::PRESSED);
	//Globals::GetInputManager()->BindInput(std::bind(&Actor::Move, this, glm::vec3(0.f, 1.f, 0.f), 1.f), MovePlatformUpKey, InputAction::REPEATED);
	Globals::GetInputManager()->BindInput(std::bind(&Actor::Move, this, glm::vec3(0.f, 1.f, 0.f), -1.f), MovePlatformDownKey, InputAction::PRESSED);
	//Globals::GetInputManager()->BindInput(std::bind(&Actor::Move, this, glm::vec3(0.f, 1.f, 0.f), -1.f), MovePlatformDownKey, InputAction::REPEATED);
}

PlayerPlatform::~PlayerPlatform()
{
}
