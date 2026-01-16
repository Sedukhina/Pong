#include "Ball.h"
#include "Engine/Assets/Generated.h"
#include "Engine/Log.h"
#include <random>

Ball::Ball(float radius, float speed)
	: Speed(speed)
{
	static std::random_device RandomDevice;
	static std::mt19937 Generator(RandomDevice());
	static std::uniform_real_distribution<float> Distribution(-1.0f, 1.0f);
	Direction = glm::normalize(glm::vec2(Distribution(Generator), Distribution(Generator)));

	std::filesystem::path MeshPath = GeneratePathForCircle(radius, Segments);
	Model PlatformModel{ MeshPath, "ink.jpg" };

	// Setting collision
	std::array<glm::vec2, 2> AABB = Globals::GetAssetManager()->GetMeshAABB(MeshPath, GetAssetID(MeshPath));
	std::shared_ptr<fcl::Spheref> CollsionSphere = std::make_shared<fcl::Spheref>(radius);
	this->AddCollision(CollsionSphere);

	this->AddModel(PlatformModel);
}

void Ball::Tick(float DeltaTime)
{
}
