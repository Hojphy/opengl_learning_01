#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

#include "Block.h"

class World
{
	public:
		void Initialize();
		void Render(Shader& shader);
		void Update(float deltaTime);
	private:
		std::vector<std::unique_ptr<Block>> m_blocks;
		void CreateCube(float x, float y, float z, float size, glm::vec3 rgb, BlockType blockType);
};

#endif