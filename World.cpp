#include "World.h"

void World::Render()
{
	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			glPushMatrix();
			glTranslatef(x * 2.0f, 0.0f, z * -2.0f);
			
		}
	}
}