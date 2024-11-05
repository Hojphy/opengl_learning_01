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
	glm::vec3 rgb;
	glm::vec3 velocity;
	
	glm::vec3 gravity;

	float size;
	float mass;
	float bounciness;
	float damping;

	BlockType blockType;
	
	Block(glm::vec3 position, float size, glm::vec3 rgb, BlockType blockType);
	~Block();
	
	void Initialize();
	void Draw(Shader& shader);
private:
	VAO* vao;
	VBO* vbo;
	EBO* ebo;

};

#endif