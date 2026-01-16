#include "Pong.h"
#include "Engine/Engine.h"
#include "Engine/Scene/Actor.h"

#include "PlayerPlatform.h"
#include "Ball.h"

int main()
{
    Engine Eng = Engine();
    Level PongLevel{};
    std::shared_ptr<PlayerPlatform> Player1 = std::make_shared<PlayerPlatform>(glm::vec3(44.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 4.f, 1.f), InputKey::UP, InputKey::DOWN);
    std::shared_ptr<PlayerPlatform> Player2 = std::make_shared<PlayerPlatform>(glm::vec3(-44.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 4.f, 1.f), InputKey::W, InputKey::S);
    PongLevel.AddActorOnLevel(Player1);
    PongLevel.AddActorOnLevel(Player2);
    std::shared_ptr<Ball> PongBall = std::make_shared<Ball>(1.f, 2.f);
    PongLevel.AddActorOnLevel(PongBall);
    Eng.Run(&PongLevel);
    return 0;
}