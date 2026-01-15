#include "SceneObject.h"

SceneObject::SceneObject()
	: Position(0.f), Scale(1.f)
{
	Rotation = glm::vec3(0.f);
	UpdateModelMatrix();
}

SceneObject::SceneObject(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: Position(position), Rotation(rotation), Scale(scale)
{
	UpdateModelMatrix();
}

SceneObject::SceneObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Position(position), Scale(scale)
{
	Rotation = glm::vec3(rotation);
	UpdateModelMatrix();
}

SceneObject::~SceneObject()
{
}

glm::vec3 SceneObject::GetPosition() const
{
	return Position;
}

void SceneObject::SetPosition(glm::vec3 newPosition)
{
	Position = newPosition;
	OnTransformChanged();
}

glm::quat SceneObject::GetRotation() const
{
	return Rotation;
}

glm::vec3 SceneObject::GetScale() const
{
	return Scale;
}

glm::mat4 SceneObject::GetModelMatrix()
{
	return ModelMatrix;
}

void SceneObject::OnTransformChanged()
{
	UpdateModelMatrix();
}

void SceneObject::UpdateModelMatrix()
{
	glm::mat4 ScaleMatrix = glm::mat4(0);
	ScaleMatrix[0][0] = Scale.x;
	ScaleMatrix[1][1] = Scale.y;
	ScaleMatrix[2][2] = Scale.z;
	ScaleMatrix[3][3] = 1;

	glm::mat4 TranslationMatrix = glm::mat4(0);
	TranslationMatrix[0][0] = 1;
	TranslationMatrix[1][1] = 1;
	TranslationMatrix[2][2] = 1;
	TranslationMatrix[3][3] = 1;
	TranslationMatrix[3][0] = Position.x;
	TranslationMatrix[3][1] = Position.y;
	TranslationMatrix[3][2] = Position.z;

	float Qw_sqr = Rotation.w * Rotation.w;
	float Qx_sqr = Rotation.x * Rotation.x;
	float Qy_sqr = Rotation.y * Rotation.y;
	float Qz_sqr = Rotation.z * Rotation.z;

	glm::mat4 RotationMatrix = glm::mat4(0);
	RotationMatrix[3][3] = 1;

	RotationMatrix[0][0] = Qw_sqr + Qx_sqr - Qy_sqr - Qz_sqr;
	RotationMatrix[0][1] = 2 * (Rotation.x * Rotation.y + Rotation.w * Rotation.z);
	RotationMatrix[0][2] = 2 * (Rotation.x * Rotation.z - Rotation.w * Rotation.y);

	RotationMatrix[1][0] = 2 * (Rotation.x * Rotation.y - Rotation.w * Rotation.z);
	RotationMatrix[1][1] = Qw_sqr - Qx_sqr + Qy_sqr - Qz_sqr;
	RotationMatrix[1][2] = 2 * (Rotation.z * Rotation.y + Rotation.w * Rotation.x);

	RotationMatrix[2][0] = 2 * (Rotation.x * Rotation.z + Rotation.w * Rotation.y);
	RotationMatrix[2][1] = 2 * (Rotation.y * Rotation.z - Rotation.w * Rotation.x);
	RotationMatrix[2][2] = Qw_sqr - Qx_sqr - Qy_sqr + Qz_sqr;;

	//TranslationMatrix* RotationMatrix* ScaleMatrix* OriginalVector;
	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
}
