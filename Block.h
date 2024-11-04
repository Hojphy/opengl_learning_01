#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vao.h"
#include "ebo.h"
#include "shaderClass.h"


class Block
{
public:
	glm::vec3 position;
	float size;
	glm::vec3 rgb;
	
	Block(glm::vec3 position, float size, glm::vec3 rgb)
		: position(position), size(size), rgb(rgb) {}
	~Block();
	
	void Initialize();
	void Draw(Shader& shader);
private:
	VAO* vao;
	VBO* vbo;
	EBO* ebo;

};

#endif