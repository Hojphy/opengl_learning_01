	#include "VAO.h"

	VAO::VAO() : ID(0), supported(false)
	{
	    if (glGenVertexArrays) {
		glGenVertexArrays(1, &ID);
		supported = true;
	    } else {
		ID = 0; // or some invalid flag
		supported = false;
	    }
	}

	void VAO::LinkVBO(VBO& vbo, GLuint layout)
	{
		if(supported)
		{
			vbo.Bind();
			glVertexAttribPointer(layout, layout == 0 ? 3 : 4, GL_FLOAT, GL_FALSE, 7 *sizeof(float), (void*)(layout == 0 ? 0 : 3 * sizeof(float)));
			glEnableVertexAttribArray(layout);
			vbo.Unbind();
		}
	}

	void VAO::Bind()
	{
		if(supported) glBindVertexArray(ID);
	}

	void VAO::Unbind()
	{
		if(supported) glBindVertexArray(0);
	}

	void VAO::Delete()
	{
		if(supported) glDeleteVertexArrays(1, &ID);
	}
