#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW\glfw3.h>

class Camera
{
	public:
		Camera();
		~Camera() = default;
		glm::mat4 GetView();

		glm::vec3 Front();
		glm::vec3 position;

		void SetPosition(glm::vec3 pos);
		void setDirection(glm::vec3 dir);
		void setUp(glm::vec3 up);
		void UpdateView();
		void Update(GLFWwindow* window, float deltaTime);
		void UpdateMouse(GLFWwindow* window, double xpos, double ypos);
		void ProcessInput(GLFWwindow *window, float deltaTime);
	private:
		glm::mat4 m_view;
		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;

		float m_yaw;
		float m_pitch;

		double m_lastx;
		double m_lasty;

		bool m_firstmouse;
};

#endif