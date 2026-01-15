#include "Actor.h"
#include "Globals.h"
#include "Log.h"

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

void Actor::AddCollision(std::shared_ptr<fcl::CollisionGeometryf> collision)
{
	ActorCollision = std::make_shared<fcl::CollisionObjectf>(collision);
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

void Actor::SetPosition(glm::vec3 newPosition)
{
	// Wall collision
	if (ActorCollision)
	{
		fcl::AABBf ActorColisionAABB =  ActorCollision->getAABB();

		if (ActorColisionAABB.min_.x() < -ScreenHalfWidth)
		{
			newPosition.x += ScreenHalfWidth - ActorColisionAABB.min_.x();
		}
		else if (ActorColisionAABB.max_.x() > ScreenHalfWidth)
		{
			newPosition.x -= ScreenHalfWidth - ActorColisionAABB.max_.x();
		}
		if (ActorColisionAABB.min_.y() < -ScreenHalfHeight)
		{
			newPosition.y += ScreenHalfWidth - ActorColisionAABB.min_.y();
		}
		else if (ActorColisionAABB.max_.y() > ScreenHalfWidth)
		{
			newPosition.y -= ScreenHalfWidth - ActorColisionAABB.max_.y();
		}
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

