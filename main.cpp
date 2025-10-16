#define _USE_MATH_DEFINES

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "shaderClass.h"
#include "Camera.h"
#include "World.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void draw(float deltaTime, Shader* shader, World* world, Camera* camera);

struct AppContext {
    Camera* camera;
    World* world;
};


int main()
{
	glfwInit();

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
	std::cout << "GL_VENDOR:   " << glGetString(GL_VENDOR) << "\n";
	std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "GL_VERSION:  " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL:        " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	glViewport(0, 0, 800, 600);


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	Shader* shader;
	const char* glVersion = (const char*)glGetString(GL_VERSION);
	if (strstr(glVersion, "3.") || strstr(glVersion, "4.")) {
		shader = new Shader("default.vert", "default.frag");
		std::cout << "Shader330\n";

	} else {
		shader = new Shader("default120.vert", "default120.frag");
		std::cout << "Shader120\n";
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	World* world = new World();
	Camera* camera = new Camera();

	AppContext* ctx = new AppContext;
	ctx->camera = camera;
	ctx->world = world;
	glfwSetWindowUserPointer(window, ctx);

	float lastFrame = glfwGetTime();
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	world->Initialize();
	while (!glfwWindowShouldClose(window))
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << err << std::endl;
		}
		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrame;
		lastFrame = currentFrameTime;
		draw(deltaTime, shader, world, camera);
		camera->Update(window, deltaTime);
		world->Update(deltaTime, camera);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
	shader->Delete();
	delete shader;
	delete camera;
	delete world;
	delete ctx;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void draw(float deltaTime, Shader* shader, World* world, Camera* camera)
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	AppContext* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
	ctx->camera->UpdateMouse(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	AppContext* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		ctx->world->LClickPress(ctx->camera);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		ctx->world->LClickRelease();
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		ctx->world->RClickPress(ctx->camera);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	AppContext* ctx = static_cast<AppContext*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		ctx->world->moveBlock = true;
	}
}
