#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include <vector>
#include <memory>

#include "Block.h"

class World
{
	public:
		void Initialize();
		void Render(Shader& shader);
	private:
		std::vector<std::unique_ptr<Block>> m_blocks;
		void CreateCube(float x, float y, float z, float size, glm::vec3 rgb);
};

#endif