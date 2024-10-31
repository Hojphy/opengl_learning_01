#define _USE_MATH_DEFINES

#include <iostream>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLfloat verticies[] = {
		//triangle
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
		//square
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f
};
std::string whatToDraw;

VAO* vao;
VBO* vbo;
Shader* shader;

void initializeVerticies()
{
	vao = new VAO();
	vbo = new VBO(verticies, sizeof(verticies));
	vao->Bind();

	VBO temp();

	vao->LinkVBO(*vbo, 0);
	vao->Unbind();
	vbo->Unbind();
}

void draw(std::string what)
{
	glClear(GL_COLOR_BUFFER_BIT);
	shader->Activate();
	double t = glfwGetTime();
	float sinWave = 0.5f * sin(1.5f * M_PI * 2.4f * t + 0.1f);
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sinWave, 0.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(sinWave, sinWave, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(transformMatrix * scaleMatrix));
	vao->Bind();
	if (what == "triangle")
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (what == "square")
	{
		glDrawArrays(GL_TRIANGLES, 3, 3);
		glDrawArrays(GL_TRIANGLES, 4, 3);
	}
	vao->Unbind();
}

int main()
{
	whatToDraw = "triangle";
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "chuj", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Okno sie zesralo\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD sie zesral\n";
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float switchTime = glfwGetTime();
	shader = new Shader("default.vert", "default.frag");
	initializeVerticies();

	while (!glfwWindowShouldClose(window))
	{
		if (whatToDraw == "triangle")
		{
			draw("triangle");
		}
		else if (whatToDraw == "square")
		{
			draw("square");
		}

		if (glfwGetTime() - switchTime >= 2.0f)
		{
			whatToDraw = whatToDraw == "triangle" ? "square" : "triangle";
			std::cout << "Drawing a " << whatToDraw << "\n";
			switchTime = glfwGetTime();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
	vao->Delete();
	vbo->Delete();
	delete vao;
	delete vbo;
	shader->Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}