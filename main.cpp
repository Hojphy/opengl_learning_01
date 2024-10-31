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
		
		//-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		//0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		//0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
		
		//square
		
		// Front face
		-0.5f, -0.5f,  0.5f, // Bottom left
		 0.5f, -0.5f,  0.5f, // Bottom right
		-0.5f,  0.5f,  0.5f, // Top left
		 0.5f,  0.5f,  0.5f, // Top right

		 // Back face
		 -0.5f, -0.5f, -0.5f, // Bottom left
		  0.5f, -0.5f, -0.5f, // Bottom right
		 -0.5f,  0.5f, -0.5f, // Top left
		  0.5f,  0.5f, -0.5f  // Top right
};

GLuint squareIndices[] = {
	// Front face
	3, 4, 5,
	4, 6, 5,

	// Left face
	3, 5, 7,
	5, 9, 7,

	// Right face
	4, 8, 6,
	8, 10, 6,

	// Bottom face
	3, 7, 4,
	4, 7, 8,

	// Top face
	5, 6, 9,
	6, 10, 9,

	// Back face
	7, 8, 9,
	8, 10, 9
};

GLuint edgeIndices[] = {
	// Front face edges
	3, 4,
	4, 6,
	6, 5,
	5, 3,

	// Back face edges
	7, 8,
	8, 10,
	10, 9,
	9, 7,

	// Connecting edges between front and back faces
	3, 7,
	4, 8,
	5, 9,
	6, 10
};

std::string whatToDraw;

VAO* vao;
VAO* edgeVao;
VBO* vbo;
EBO* ebo;
EBO* edgeEbo;
Shader* shader;
float rotation = 0;

void initializeVerticies()
{
	vao = new VAO();
	vbo = new VBO(verticies, sizeof(verticies));
	vao->Bind();

	ebo = new EBO(squareIndices, sizeof(squareIndices));
	ebo->Bind();
	vao->LinkVBO(*vbo, 0);
	vao->Unbind();
	ebo->Unbind();
	edgeVao = new VAO();

	edgeEbo = new EBO(edgeIndices, sizeof(edgeIndices));
	edgeEbo->Bind();
	edgeVao->LinkVBO(*vbo, 0);

	vbo->Unbind();
	edgeEbo->Unbind();
}

void draw(std::string what)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Activate();
	double t = glfwGetTime();
	float sinWave = 1.5f * sin(1.5f * M_PI * 2.4f * t + 0.1f);
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 Model = glm::mat4(1.0f);

	Model = glm::rotate(Model, glm::radians((float)rotation), glm::vec3(fabs(sinWave), fabs(-sinWave), fabs(sinWave)));


	glm::mat4 View = glm::lookAt(
		glm::vec3(5, 1, -3), // Camera viewpoint pos
		glm::vec3(0, 0, 0),		   // Looking pos
		glm::vec3(0, 1, 0)		   // Up direction
	);

	glm::mat4 Projection = glm::perspective(
		glm::radians(45.0f), //FoV
		4.0f / 3.0f,         //Aspect Ratio
		0.1f,                //Near clipping plane
		100.0f               //Far clipping plane
	);

	glm::mat4 mvp = Projection * View * Model;

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(mvp));
	if (what == "triangle")
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (what == "square")
	{
		vao->Bind();
		glDrawElements(GL_TRIANGLES, sizeof(squareIndices), GL_UNSIGNED_INT, nullptr);
		//glColor3f(0.8f, 0.3f, 0.02f);
		float myColor[] = { sinWave, 0.5f, sinWave, 1.0f };
		GLint colorLocation = glGetUniformLocation(shader->ID, "color");
		glUniform4fv(colorLocation, 1, myColor);
		vao->Unbind();
		edgeVao->Bind();

		glDrawElements(GL_LINES, sizeof(edgeIndices), GL_UNSIGNED_INT, (void*)0);
		float myColor2[] = { -sinWave, 0.8f, -sinWave, 1.0f };
		colorLocation = glGetUniformLocation(shader->ID, "color");
		glUniform4fv(colorLocation, 1, myColor2);

		glLineWidth(2.0f);
		edgeVao->Unbind();
	}
}

int main()
{
	bool swapShapes = false;
	whatToDraw = "square";
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
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float rotationTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - rotationTime;
		rotationTime = currentFrameTime;

		float rotationSpeed = 90.0f;
		rotation += rotationSpeed * deltaTime;
		rotation = fmod(rotation, 360.0f);
		draw(whatToDraw);
		if (glfwGetTime() - switchTime >= 2.0f && swapShapes)
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