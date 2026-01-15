#pragma once

#include "SceneObject.h"

// Collision library
#include <fcl/fcl.h>
#include "Assets/Model.h"
#include <filesystem>
#include <memory>
#include <typeinfo>

class Actor : public SceneObject
{
public:
	Actor();
	Actor(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Actor();

	void AddModel(Model& newModel);
	const std::vector<std::shared_ptr<Model>>& GetActorsModels() const;
	void Move(glm::vec3 direction, float step);
	
	virtual void OnCollision(Actor* OtherActor) {};

private:
	std::vector<std::shared_ptr<Model>> ActorModels;
	std::shared_ptr<fcl::CollisionGeometryd> ActorCollision;
};

