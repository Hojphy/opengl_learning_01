#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
	public:
		GLuint ID;
		VAO();
		bool supported;
		void LinkVBO(VBO& vbo, GLuint layout);
		void Bind();
		void Unbind();
		void Delete();
};

#endif
