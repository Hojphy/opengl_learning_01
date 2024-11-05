#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Block.h"
#include "Camera.h"

class World
{
	public:
		void Initialize();
		void Render(Shader& shader);
		void Update(float deltaTime, Camera* camera);
		void LClickPress(Camera* camera);
		void LClickRelease();
		
		Block* InFrontOfCamera(Camera* camera);

		bool moveBlock;
		Block* selectedBlock;
	private:
		std::vector<std::unique_ptr<Block>> m_blocks;
		void CreateCube(float x, float y, float z, float size, glm::vec3 rgb, BlockType blockType);
};

#endif