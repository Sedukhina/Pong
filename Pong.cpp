#include "Pong.h"
#include "Engine/Engine.h"
#include "Engine/Scene/Actor.h"

int main()
{
    Engine Eng = Engine();
    Level PongLevel{};
    Model TestModel {"DonutPainting.fbx", "ink.jpg"};
    std::shared_ptr<Actor> Triangle = std::make_shared<Actor>();
    Triangle->AddModel(TestModel);
    std::shared_ptr<Actor> Triangle2 = std::make_shared<Actor>(glm::vec3(20.f), glm::vec3(0.f), glm::vec3(7.f));
    Model TestModel2{"DonutPainting.fbx", "DonutPainting_Frame_AlbedoTransparency.jpg" };
    Triangle2->AddModel(TestModel2);
    PongLevel.AddActorOnLevel(Triangle);
    PongLevel.AddActorOnLevel(Triangle2);
    Eng.Run(&PongLevel);
    return 0;
}