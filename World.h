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
		void RClickPress(Camera* camera);
		bool RayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
			const glm::vec3& boxMin, const glm::vec3& boxMax, float maxDistance);
		
		Block* InFrontOfCamera(Camera* camera);

		bool moveBlock;
		Block* selectedBlock;
	private:
		std::vector<std::unique_ptr<Block>> m_blocks;
		void CreateCube(float x, float y, float z, float size, glm::vec3 rgb, BlockType blockType);
};

#endif