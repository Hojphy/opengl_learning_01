	#include "VAO.h"

	VAO::VAO()
	{
		glGenVertexArrays(1, &ID);
	}

	void VAO::LinkVBO(VBO vbo, GLuint layout)
	{
		vbo.Bind();
		glVertexAttribPointer(layout, layout == 0 ? 3 : 4, GL_FLOAT, GL_FALSE, (layout == 0 ? 3 : 4) *sizeof(float), (void*)0);
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