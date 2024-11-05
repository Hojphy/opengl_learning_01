#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	position = glm::vec3(0, 0, 3.0f);
	m_direction = glm::vec3(0, 0, 0);
	m_up = glm::vec3(0, 1.0f, 0);
	m_view = glm::lookAt(
		position,
		m_direction,
		m_up
	);
	m_yaw = -90;
	m_pitch = 0;
	m_lastx = 400;
	m_lasty = 300;
	m_firstmouse = true;
}

void Camera::Update(GLFWwindow *window, float deltaTime)
{
	ProcessInput(window, deltaTime);
	UpdateView();
}

void Camera::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void Camera::setUp(glm::vec3 up)
{
	m_up = up;
}

void Camera::UpdateView()
{
	m_view = glm::lookAt(
		position,
		position + Front(),
		m_up
	);
}

void Camera::UpdateMouse(GLFWwindow* window, double xpos, double ypos)
{
	if (m_firstmouse)
	{
		m_lastx = xpos;
		m_lasty = ypos;
		m_firstmouse = false;
	}

	float xoffset = xpos - m_lastx;
	float yoffset = m_lasty - ypos;
	m_lastx = xpos;
	m_lasty = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;


	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	m_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_direction.y = sin(glm::radians(m_pitch));
	m_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_right = glm::normalize(glm::cross(Front(), glm::vec3(0,1.0f,0)));
	m_up = glm::normalize(glm::cross(m_right, Front()));
}

void Camera::ProcessInput(GLFWwindow *window, float deltaTime)
{
	const float speed = 2 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += speed * Front();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= speed * Front();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= glm::normalize(glm::cross(Front(), m_up)) * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += glm::normalize(glm::cross(Front(), m_up)) * speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		position.y += speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		position.y -= speed;
		
}

glm::mat4 Camera::GetView()
{
	return m_view;
}

glm::vec3 Camera::Front()
{
	return glm::normalize(m_direction);
}