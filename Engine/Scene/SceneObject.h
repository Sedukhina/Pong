#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>


class SceneObject
{
public:
	SceneObject();
	SceneObject(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
	SceneObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~SceneObject();

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetScale() const;

	glm::mat4 GetModelMatrix();


protected:
	void SetPosition(glm::vec3 newPosition);

private:
	void UpdateModelMatrix();

	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	glm::mat4 ModelMatrix;
};

