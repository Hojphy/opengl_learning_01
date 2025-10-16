#include "World.h"
#include "Physics.h"

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
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->blockType == PHYSICS)
		{
			if (selectedBlockIdx == -1 && InFrontOfCamera(camera) == -1 && i == InFrontOfCamera(camera))
			{
				m_blocks[i]->SetRGB(glm::vec3(1, 1, 0));
				selectedBlockDistance = -1;
			}
			else if (selectedBlockIdx == -1)
			{
				m_blocks[i]->SetRGB(glm::vec3(1, 1, 1));
				selectedBlockDistance = -1;
			}
			if (selectedBlockIdx != -1)
			{
				if (selectedBlockDistance == -1)
				{
					selectedBlockDistance = sqrt(pow(m_blocks[selectedBlockIdx]->position.x - camera->position.x, 2) + pow(m_blocks[selectedBlockIdx]->position.y - camera->position.y, 2) + pow(m_blocks[selectedBlockIdx]->position.z - camera->position.z, 2));
				}
				m_blocks[selectedBlockIdx]->SetRGB(glm::vec3(0.5, 0.5, 0));
				m_blocks[selectedBlockIdx]->position = camera->position + camera->Front() * selectedBlockDistance;
				m_blocks[selectedBlockIdx]->previousPosition = m_blocks[selectedBlockIdx]->position;
			}
			if (moveBlock)
			{
				m_blocks[i]->previousPosition.y = m_blocks[i]->position.y + 1.0f;
				m_blocks[i]->position.y += 1.0f;
			}
			glm::vec3 acceleration = m_blocks[i]->gravity;
			glm::vec3 newPosition = m_blocks[i]->position + (m_blocks[i]->position - m_blocks[i]->previousPosition) + acceleration * deltaTime * deltaTime;
			m_blocks[i]->previousPosition = m_blocks[i]->position;
			m_blocks[i]->position = newPosition;
			Physics::UpdateCollision(this, m_blocks[i].get());
		}
	}
	moveBlock = false;
}

void World::LClickPress(Camera* camera)
{
	int inFrontOfCamera = InFrontOfCamera(camera);
	if (inFrontOfCamera!=-1)
	{
		selectedBlockIdx = inFrontOfCamera;
	}
}

void World::LClickRelease()
{
	selectedBlockIdx = -1;
}

void World::RClickPress(Camera* camera)
{
	glm::vec3 pos = camera->position + camera->Front() * 2.0f;
	CreateCube(pos.x, pos.y, pos.z, 1.0f, glm::vec3(1, 1, 1), PHYSICS);
}

void World::CreateCube(float x, float y, float z, float size, glm::vec3 rgb, BlockType blockType)
{
	auto block = std::make_unique<Block>(glm::vec3(x, y, z), size, rgb, blockType);
	block->Initialize();
	m_blocks.push_back(std::move(block));
}

int World::InFrontOfCamera(Camera* camera)
{
	const float maxDistance = 5.0f;
	glm::vec3 origin = camera->position;
	glm::vec3 direction = camera->Front();
	Block* blok;
	for (const auto& block : m_blocks)
	{
		if (block->blockType != PHYSICS) continue;
		for (float i = 1; i <= maxDistance; i++)
		{
			float halfSize = block->size / 2;
			glm::vec3 blockMin = block->position - glm::vec3(halfSize);
			glm::vec3 blockMax = block->position + glm::vec3(halfSize);
			if (RayIntersectsAABB(origin, direction, blockMin, blockMax, maxDistance))
			{
				blok = block.get();
				break;
			}
			if(blok) break;
		}
	}
	if (blok) {
	    for (size_t i = 0; i < m_blocks.size(); ++i) {
		if (m_blocks[i].get() == blok) {
		    return (int)i;
		}
	    }
	}
	return -1;
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
