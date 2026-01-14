#include "Engine/Scene/Actor.h"
#include "Input/InputTypes.h"

class PlayerPlatform : public Actor
{
public:
	PlayerPlatform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, InputKey UpKey, InputKey DownKey);
	~PlayerPlatform();

protected:
	InputKey MovePlatformUpKey;
	InputKey MovePlatformDownKey;
};