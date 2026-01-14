#include "Actor.h"
#include <glm/gtx/norm.hpp> 

Actor::Actor() :
	SceneObject(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(1.f))
{

}

Actor::Actor(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: SceneObject(position, rotation, scale)
{
}

Actor::~Actor()
{
}

void Actor::AddModel(Model &newModel)
{
	ActorModels.push_back(std::make_shared<Model>(newModel));
}

const std::vector<std::shared_ptr<Model>>& Actor::GetActorsModels() const
{
	return ActorModels;
}

void Actor::Move(glm::vec3 direction, float step)
{
	if (glm::length2(direction) < 1e-4f)
		return;

	direction = glm::normalize(direction);
	SetPosition(GetPosition() + direction * step);
}
