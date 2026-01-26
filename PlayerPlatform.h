#include "Engine/Scene/Actor.h"
#include "Input/InputTypes.h"

class PlayerPlatform : public Actor
{
public:
	PlayerPlatform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, InputKey UpKey, InputKey DownKey);

	virtual void Tick(float DeltaTime) override;
	void RequestPlatformMovement(glm::vec3 direction);

protected:
	void MovePlatform(glm::vec3 direction, float step);

	float PlatformSpeed = 250.f;

	glm::vec3 RequestedDirection = glm::vec3();

	InputKey MovePlatformUpKey = InputKey::W;
	InputKey MovePlatformDownKey = InputKey::S;
};