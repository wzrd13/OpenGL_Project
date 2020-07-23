#include "Movement.h"
#include <iostream>


Movement::Movement(GLFWwindow *window)
	: m_Window(window)
{
	glfwGetWindowSize(m_Window, &m_Width, &m_Height);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void  Movement::OnUpdate()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	glfwGetCursorPos(m_Window, &m_xpos, &m_ypos);

	if(!escape_flag)
	{
		glfwSetCursorPos(m_Window, (double)m_Width / 2, (double)m_Height / 2);

		m_HorizontalAngle += m_MouseSpeed * deltaTime * float(m_Width / 2 - m_xpos);
		m_VerticalAngle += m_MouseSpeed * deltaTime * float(m_Height / 2 - m_ypos);
	}

	m_Direction = glm::vec3(
		cos(m_VerticalAngle) * sin(m_HorizontalAngle), 
		sin(m_VerticalAngle), 
		cos(m_VerticalAngle) * cos(m_HorizontalAngle));

	glm::vec3 right(
		sin(m_HorizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_HorizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, m_Direction);

	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Position += m_Direction * deltaTime * m_Speed;
		//std::cout << "W" << std::endl;
	}
	// Move backward
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Position -= m_Direction * deltaTime * m_Speed;
		//std::cout << "S" << std::endl;
	}
	// Strafe right
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Position += right * deltaTime * m_Speed;
		//std::cout << "D" << std::endl;
	}
	// Strafe left
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Position -= right * deltaTime * m_Speed;
		//std::cout << "A" << std::endl;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_Position += up * deltaTime * m_Speed;
		//std::cout << "SPACE" << std::endl;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_Position -= up * deltaTime * m_Speed;
		//std::cout << "SHIFT" << std::endl;
	}

	//// Task 5.6: handle zoom in/out effects
	//if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS) {
	//	m_InitialFoV -= m_FovSpeed;
	//}
	//if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	//	m_InitialFoV += m_FovSpeed;
	//}

	lastTime = currentTime;
}

glm::vec3 Movement::GetDirection()
{
	return m_Direction;
}

glm::vec3 Movement::GetPosition()
{
	return m_Position;
}

void Movement::SetEscapeFlag()
{
	escape_flag = !escape_flag;
	glfwSetCursorPos(m_Window, (double)m_Width / 2, (double)m_Height / 2);
	if(escape_flag == 0)
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


