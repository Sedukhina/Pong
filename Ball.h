#pragma once
#include "Engine/Scene/Actor.h"
#include <filesystem>
#include "PongPlayer.h"

class Ball : public Actor
{
public:
	Ball(float radius, float speed);
	void BindFunctionOnEndRound(std::function<void(PongPlayer)> func);
	virtual void Tick(float DeltaTime) override;

protected:
	void StartRound();

	void EndRound(PongPlayer PlayerWinner);
	std::vector<std::function<void(PongPlayer)>> OnRoundEndBindedFunctions;

	void MoveBall(const std::vector<std::shared_ptr<Actor>> &ActorsOnLevel, float Step);
	// Returns contact normal
	glm::vec2 Ball::CheckCollisionWithActors(const std::vector<std::shared_ptr<Actor>>& ActorsOnLevel, float Step, float* Distance);
	// Returns which directions were overlapped
	std::array<bool, 2> Ball::CheckCollisionWithWalls(float Step, float* Distance);

	const int Segments = 32;
	const float Radius;
	float Speed = 0.f;
	glm::vec2 Direction = glm::vec2(0.f);
};