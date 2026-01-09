// Stores objects on level
#include "Actor.h"
#include <vector>

class Level
{
public:
	Level();
	~Level();

private:
	std::vector<Actor*> ActorsOnLevel;
};

