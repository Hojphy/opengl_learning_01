#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Block
{
public:
	float x, y;
	float size;
	glm::vec3 rgb;
	
	Block(float x, float y, float size, glm::vec3 rgb)
		: x(x), y(y), size(size), rgb(rgb) {}
	
	void Draw();


};

#endif