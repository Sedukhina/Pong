#pragma once

#include "SceneObject.h"

// Collision library
#include <fcl/fcl.h>
#include "Assets/Model.h"
#include <memory>

class Actor : public SceneObject
{
public:
	Actor();
	Actor(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	
	virtual void Tick(float DeltaTime) {};

	void AddModel(std::shared_ptr<Model> NewModel);
	void AddCollision(std::shared_ptr<fcl::CollisionGeometryf> collision);

	const std::vector<std::shared_ptr<Model>>& GetActorsModels() const;
	const std::shared_ptr<fcl::CollisionObjectf>& GetActorCollision() const;

	// Checks only wall collision
	virtual void SetPosition(glm::vec3 newPosition) override;
	virtual void OnTransformChanged() override;

private:
	std::vector<std::shared_ptr<Model>> ActorModels;
	std::shared_ptr<fcl::CollisionObjectf> ActorCollision;
};

