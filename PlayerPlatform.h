#include "Engine/Scene/Actor.h"
#include "Input/InputTypes.h"

class PlayerPlatform : public Actor
{
public:
	PlayerPlatform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, InputKey UpKey, InputKey DownKey);

	void MovePlatform(glm::vec3 direction, float speed);

protected:
	float PlatformSpeed = 5.f;

	InputKey MovePlatformUpKey;
	InputKey MovePlatformDownKey;

	inline static const auto MeshPath = "DonutPainting.fbx";
	inline static const auto TexturePath = "Donut_tex.jpg";
};