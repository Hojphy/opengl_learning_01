#include "World.h"

void World::Initialize()
{
	//3x3 block floor
	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			CreateCube(x, -2.0f, z, 1.0f, glm::vec3(fabs(x), 0.5f, fabs(z)));
		}
	}
}

void World::Render(Shader& shader)
{
	for (const auto& block : m_blocks)
	{
		block->Draw(shader);
	}
}

void World::CreateCube(float x, float y, float z, float size, glm::vec3 rgb)
{
	auto block = std::make_unique<Block>(glm::vec3(x, y, z), size, rgb);
	block->Initialize();
	m_blocks.push_back(std::move(block));
}