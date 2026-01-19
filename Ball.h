#pragma once
#include "Engine/Scene/Actor.h"
#include <filesystem>

class Ball : public Actor
{
public:
	Ball(float radius, float speed);
	void BindFunctionOnEndRound(std::function<void(int)> func);
	virtual void Tick(float DeltaTime) override;

protected:
	void StartRound();

	void EndRound(bool PlayerWinner);
	std::vector<std::function<void(int)>> OnRoundEndBindedFunctions;

	void MoveBall(const std::vector<std::shared_ptr<Actor>> &ActorsOnLevel, float Step);
	// Returns contact normal
	glm::vec2 Ball::CheckCollisionWithActors(const std::vector<std::shared_ptr<Actor>>& ActorsOnLevel, float Step, float* Distance);
	// Returns which directions were overlapped
	std::array<bool, 2> Ball::CheckCollisionWithWalls(float Step, float* Distance);

	const int Segments = 32;
	const float Radius;
	float Speed = 0.f;
	glm::vec2 Direction;

	std::filesystem::path EndRoundSound = "LostRound.mp3";
	std::filesystem::path PlatformSound = "Platform.mp3";
};