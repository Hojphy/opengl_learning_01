#include "World.h"

void World::Initialize()
{
	//3x3 block floor
	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			CreateCube(x, -2.0f, z, 1.0f, glm::vec3(fabs(x), 0.5f, fabs(z)), SOLID);
		}
	}
	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			CreateCube(x, 10.0f, z, 1.0f, glm::vec3(fabs(x), 0.5f, fabs(z)), SOLID);
		}
	}
	CreateCube(0.2f, 9, 0.5f, 1.0f, glm::vec3(1, 1, 1), PHYSICS);
}

void World::Render(Shader& shader)
{
	for (const auto& block : m_blocks)
	{
		block->Draw(shader);
	}
}

void World::Update(float deltaTime)
{
	for (const auto& block : m_blocks)
	{
		if (block->blockType == PHYSICS)
		{
			glm::vec3 acceleration = block->gravity;
			glm::vec3 newPosition = block->position + (block->position - block->previousPosition) + acceleration * deltaTime * deltaTime;

			block->previousPosition = block->position;
			block->position = newPosition;

			for (const auto& block2 : m_blocks)
			{
				if (block == block2) continue;
				float halfSize = block->size/2;

				float minY = block2->position.y - halfSize + 0.5f;
				float maxY = block2->position.y + halfSize + 0.5f;
				if (block->position.x <= block2->position.x + halfSize &&
					block->position.x >= block2->position.x - halfSize &&
					block->position.z <= block2->position.z + halfSize &&
					block->position.z >= block2->position.z - halfSize &&
					block->position.y <= maxY && block->position.y >= minY)
				{
					block->position.y = block2->position.y + 1;
					block->previousPosition.y = block2->position.y + 1;
					break;
				}

			}
		}
	}
}

void World::CreateCube(float x, float y, float z, float size, glm::vec3 rgb, BlockType blockType)
{
	auto block = std::make_unique<Block>(glm::vec3(x, y, z), size, rgb, blockType);
	block->Initialize();
	m_blocks.push_back(std::move(block));
}