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
#include "Camera.h"
#include "World.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Shader* shader;
Camera* camera;
World* world;

void draw(float deltaTime)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Model = glm::mat4(1.0f);

	glm::mat4 View = camera->GetView();

	glm::mat4 Projection = glm::perspective(
		glm::radians(45.0f), //FoV
		4.0f / 3.0f,         //Aspect Ratio
		0.1f,                //Near clipping plane
		100.0f               //Far clipping plane
	);

	shader->Activate();

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(Model));

	world->Render(*shader);
}

int main()
{
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
	glfwSetCursorPosCallback(window, mouse_callback);

	shader = new Shader("default.vert", "default.frag");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	world = new World();
	world->Initialize();

	camera = new Camera();
	float lastFrame = glfwGetTime();
	glfwSetKeyCallback(window, key_callback);
	while (!glfwWindowShouldClose(window))
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << err << std::endl;
		}
		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrame;
		lastFrame = currentFrameTime;
		draw(deltaTime);
		camera->Update(window, deltaTime);
		world->Update(deltaTime);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
	shader->Delete();
	delete shader;
	delete camera;
	delete world;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->UpdateMouse(window, xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		world->moveBlock = true;
	}
}