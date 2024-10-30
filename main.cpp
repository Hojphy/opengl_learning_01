#define _USE_MATH_DEFINES

#include <iostream>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"   gl_Position = model * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

GLfloat triangleVerticies[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
};
GLfloat squareVerticies[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f 
};
GLuint shaderProgram;
GLuint vertexArrayObject, vertexBufferObject;
std::string whatToDraw;

void initializeShaders()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void initializeVerticies()
{
	glGenVertexArrays(1, &vertexArrayObject);

	glGenBuffers(1, &vertexBufferObject);

	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void draw(std::string what)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertexArrayObject);
	glUseProgram(shaderProgram);
	double t = glfwGetTime();
	float sinWave = 0.5f * sin(1.5f * M_PI * 2.4f * t + 0.1f);
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-sinWave, sinWave, 0.0f));
	if (what == "triangle")
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(transformMatrix));
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerticies), triangleVerticies, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (what == "square")
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(transformMatrix));
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareVerticies), squareVerticies, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 1, 3);
	}
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

	initializeShaders();
	initializeVerticies();

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float switchTime = glfwGetTime();

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
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}