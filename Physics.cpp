#include "Physics.h"

void Physics::UpdateCollision(World* world, Block* block)
{
	float halfSize = block->size / 2;
	for (const auto& block2 : world->m_blocks)
	{
		if (block == block2.get()) continue;

        GroundBlocks(block, block2.get());

        float min1X = block->position.x - halfSize;
        float max1X = block->position.x + halfSize;
        float min1Y = block->position.y - halfSize;
        float max1Y = block->position.y + halfSize;
        float min1Z = block->position.z - halfSize;
        float max1Z = block->position.z + halfSize;

        float min2X = block2->position.x - halfSize;
        float max2X = block2->position.x + halfSize;
        float min2Y = block2->position.y - halfSize;
        float max2Y = block2->position.y + halfSize;
        float min2Z = block2->position.z - halfSize;
        float max2Z = block2->position.z + halfSize;

        bool xCollision = max1X >= min2X && min1X <= max2X;
        bool yCollision = max1Y >= min2Y && min1Y <= max2Y;
        bool zCollision = max1Z >= min2Z && min1Z <= max2Z;

        if (xCollision && yCollision && zCollision) {
            float overlapX = std::min(max1X - min2X, max2X - min1X);
            float overlapY = std::min(max1Y - min2Y, max2Y - min1Y);
            float overlapZ = std::min(max1Z - min2Z, max2Z - min1Z);

            if (overlapX < overlapY && overlapX < overlapZ) {
                if (block->position.x < block2->position.x) {
                    block->position.x -= overlapX + 0.0001f;
                }
                else {
                    block->position.x += overlapX + 0.0001f;
                }
            }
            else if (overlapY < overlapX && overlapY < overlapZ && !block->grounded) {
                if (block->position.y < block2->position.y) {
                    block->position.y -= overlapY + 0.0001f;
                }
                else {
                    block->position.y += overlapY + 0.0001f;
                }
            }
            else if(overlapZ < overlapX && overlapZ < overlapY) {
                if (block->position.z < block2->position.z) {
                    block->position.z -= overlapZ + 0.0001f;
                }
                else {
                    block->position.z += overlapZ + 0.0001f;
                }
            }

            block->previousPosition = block->position;
        }
	}
}

void Physics::GroundBlocks(Block* block, Block* otherBlock)
{
    float halfSize = block->size / 2;
    float otherHalfSize = otherBlock->size / 2;

    float min1X = block->position.x - halfSize;
    float max1X = block->position.x + halfSize;
    float min1Y = block->position.y - halfSize;
    float max1Y = block->position.y + halfSize;
    float min1Z = block->position.z - halfSize;
    float max1Z = block->position.z + halfSize;

    float min2X = otherBlock->position.x - otherHalfSize;
    float max2X = otherBlock->position.x + otherHalfSize;
    float min2Y = otherBlock->position.y - otherHalfSize;
    float max2Y = otherBlock->position.y + otherHalfSize;
    float min2Z = otherBlock->position.z - otherHalfSize;
    float max2Z = otherBlock->position.z + otherHalfSize;

    bool xCollision = max1X >= min2X && min1X <= max2X;
    bool yCollision = max1Y >= min2Y && min1Y <= max2Y;
    bool zCollision = max1Z >= min2Z && min1Z <= max2Z;

    if (yCollision && xCollision && zCollision)
    {
        block->grounded = true;
    }
    else
    {
        block->grounded = false;
    }
}