#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>


class SceneObject
{
public:
	SceneObject();
	SceneObject(glm::vec3 ActorPosition, glm::quat ActorRotation, glm::vec3 ActorScale);
	SceneObject(glm::vec3 ActorPosition, glm::vec3 ActorRotation, glm::vec3 ActorScale);

	const glm::vec3& GetPosition() const;
	const glm::quat& GetRotation() const;
	const glm::vec3& GetScale() const;

	const glm::mat4& GetModelMatrix() const;

	virtual ~SceneObject() = default;

protected:
	virtual void SetPosition(glm::vec3 newPosition);
	virtual void OnTransformChanged();

private:
	void UpdateModelMatrix();

	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	glm::mat4 ModelMatrix;
};

