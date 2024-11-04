#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Block.h"

class World
{
	public:
		void Initialize();
		void Render();
	private:
		std::vector<Block> m_blocks;
		void CreateCube(float x, float y, float size, glm::vec3 rgb);
};

#endif