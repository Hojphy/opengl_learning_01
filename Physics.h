#ifndef PHYSICS_CLASS_H
#define PHYSICS_CLASS_H

#include "World.h"

class Physics
{
public:
	static void UpdateCollision(World* world, Block* block);
};

#endif