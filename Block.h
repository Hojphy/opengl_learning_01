#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vao.h"
#include "ebo.h"
#include "shaderClass.h"

enum BlockType
{
	SOLID,
	PHYSICS
};

class Block
{
public:
	glm::vec3 previousPosition;
	glm::vec3 position;
	glm::vec3 velocity;
	
	glm::vec3 gravity;

	float size;
	float mass;
	float bounciness;
	float damping;

	bool grounded;

	BlockType blockType;
	
	Block(glm::vec3 position, float size, glm::vec3 rgb, BlockType blockType);
	Block()
		: position(0.0f), size(1.0f), m_rgb(1.0f, 1.0f, 1.0f), blockType(SOLID),
		damping(0.99f), mass(1.0f), gravity(0.0f, -9.81f, 0.0f),
		bounciness(0.8f), velocity(0.0f), previousPosition(0.0f) {}

	~Block();
	
	void Initialize();
	void Draw(Shader& shader);
	void SetRGB(glm::vec3 rgb);
	glm::vec3 GetRGB();
private:
	VAO* vao;
	VBO* vbo;
	EBO* ebo;
	glm::vec3 m_rgb;
};

#endif