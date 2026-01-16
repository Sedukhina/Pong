#pragma once
#include "Engine/Scene/Actor.h"

class Ball : public Actor
{
public:
	Ball(float radius, float speed);

	virtual void Tick(float DeltaTime) override;

protected:
	const int Segments = 32;
	float Speed = 0.f;
	glm::vec2 Direction;
};