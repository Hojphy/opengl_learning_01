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

void World::Update(float deltaTime, Camera* camera)
{
	for (const auto& block : m_blocks)
	{
		if (block->blockType == PHYSICS)
		{
			if (InFrontOfCamera(camera))
			{
				block->SetRGB(glm::vec3(1, 1, 0));
			}
			else
			{
				block->SetRGB(glm::vec3(1, 1, 1));
			}
			if (selectedBlock)
			{
				if (block.get() == selectedBlock)
				{
					block->SetRGB(glm::vec3(0.5, 0.5, 0));
					block->position = camera->position + camera->Front() * 2.0f;
					block->previousPosition = block->position;
					continue;
				}
				else
				{
					block->SetRGB(glm::vec3(1, 1, 1));
				}
			}
			if (moveBlock)
			{
				block->previousPosition.y = block->position.y+1.0f;
				block->position.y += 1.0f;
				moveBlock = false;
			}
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

void World::LClickPress(Camera* camera)
{
	Block* inFrontOfCamera = InFrontOfCamera(camera);
	if (inFrontOfCamera)
	{
		selectedBlock = inFrontOfCamera;
	}
}

void World::LClickRelease()
{
	selectedBlock = nullptr;
}

void World::CreateCube(float x, float y, float z, float size, glm::vec3 rgb, BlockType blockType)
{
	auto block = std::make_unique<Block>(glm::vec3(x, y, z), size, rgb, blockType);
	block->Initialize();
	m_blocks.push_back(std::move(block));
}

Block* World::InFrontOfCamera(Camera* camera)
{
	const float maxDistance = 5.0f;
	glm::vec3 origin = camera->position;
	glm::vec3 direction = camera->Front();
	for (const auto& block : m_blocks)
	{
		if (block->blockType != PHYSICS) continue;
		const unsigned int range = 5;
		for (float i = 1; i <= range; i++)
		{
			float halfSize = block->size / 2;
			glm::vec3 blockMin = block->position - glm::vec3(halfSize);
			glm::vec3 blockMax = block->position + glm::vec3(halfSize);
			if (RayIntersectsAABB(origin, direction, blockMin, blockMax, maxDistance))
			{
				return block.get();
			}
		}
	}
	return nullptr;
}

bool World::RayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
	const glm::vec3& boxMin, const glm::vec3& boxMax, float maxDistance)
{
	float tmin = 0.0f;
	float tmax = maxDistance;

	for (int i = 0; i < 3; ++i)
	{
		float invD = 1 / rayDirection[i];
		float t0 = (boxMin[i] - rayOrigin[i]) * invD;
		float t1 = (boxMax[i] - rayOrigin[i]) * invD;

		if (invD < 0.0f)
			std::swap(t0, t1);

		tmin = (t0 > tmin) ? t0 : tmin;
		tmax = (t1 < tmax) ? t1 : tmax;

		if (tmin > tmax || tmax < 0)
			return false;
	}

	return true;
}