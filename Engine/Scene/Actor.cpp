#include "Actor.h"
#include "Globals.h"
#include "Log.h"

Actor::Actor() :
	SceneObject(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(1.f))
{

}

Actor::Actor(glm::vec3 ActorPosition, glm::vec3 ActorRotation, glm::vec3 ActorScale)
	: SceneObject(ActorPosition, ActorRotation, ActorScale)
{
}

void Actor::AddModel(std::shared_ptr<Model> NewModel)
{
	ActorModels.push_back(NewModel);
}

void Actor::AddCollision(std::shared_ptr<fcl::CollisionGeometryf> NewCollision)
{
	ActorCollision = std::make_shared<fcl::CollisionObjectf>(NewCollision);
	glm::vec3 Pos = GetPosition();
	ActorCollision->setTranslation(fcl::Vector3f(Pos.x, Pos.y, Pos.z));
	glm::quat Rotation = GetRotation();
	ActorCollision->setQuatRotation(fcl::Quaternionf(Rotation.w, Rotation.x, Rotation.y, Rotation.z));
	ActorCollision->computeAABB();
}

const std::vector<std::shared_ptr<Model>>& Actor::GetActorsModels() const
{
	return ActorModels;
}

const std::shared_ptr<fcl::CollisionObjectf>& Actor::GetActorCollision() const
{
	return ActorCollision;
}

void Actor::SetPosition(glm::vec3 newPosition)
{
	if (ActorCollision)
	{
		ActorCollision->setTranslation(fcl::Vector3f(newPosition.x, newPosition.y, newPosition.z));
	}
	SceneObject::SetPosition(newPosition);
}

void Actor::OnTransformChanged()
{
	SceneObject::OnTransformChanged();

	if (ActorCollision)
	{
		ActorCollision->computeAABB();
	}
}

