	#include "VAO.h"

	VAO::VAO()
	{
		glGenVertexArrays(1, &ID);
	}

	void VAO::LinkVBO(VBO vbo, GLuint layout)
	{
		vbo.Bind();
		glVertexAttribPointer(layout, layout == 0 ? 3 : 4, GL_FLOAT, GL_FALSE, 7 *sizeof(float), (void*)(layout == 0 ? 0 : 3 * sizeof(float)));
		glEnableVertexAttribArray(layout);
		vbo.Unbind();
	}

	void VAO::Bind()
	{
		glBindVertexArray(ID);
	}

	void VAO::Unbind()
	{
		glBindVertexArray(0);
	}

	void VAO::Delete()
	{
		glDeleteVertexArrays(1, &ID);
	}