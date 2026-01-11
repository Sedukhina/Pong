#pragma once
#include "SceneObject.h"
#include "Assets/Model.h"
#include <filesystem>
#include <memory>

class Actor : public SceneObject
{
public:
	Actor();
	Actor(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Actor();

	void AddModel(Model& newModel);
	const std::vector<std::shared_ptr<Model>>& GetActorsModels() const;

private:
	std::vector<std::shared_ptr<Model>> ActorModels;
	// class Collsion* ActorCollision
};

