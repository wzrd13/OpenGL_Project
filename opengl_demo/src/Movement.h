#pragma once

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

class Movement
{
private:
	int m_Width, m_Height;
	double m_xpos, m_ypos;

	glm::vec3 m_Direction;
	glm::vec3 m_Position;

	GLFWwindow *m_Window;
	bool escape_flag = 0;
	glm::vec3 position;
	float m_HorizontalAngle = 3.14f;
	float m_VerticalAngle = 0.0f;
	float m_InitialFoV = 45.0f;
	float m_FovSpeed = 2.0f;
	float m_Speed = 5.0f;
	float m_MouseSpeed = 0.05f;
public:
	Movement(GLFWwindow *window);
	void OnUpdate();

	glm::vec3 GetDirection();
	glm::vec3 GetPosition();
	void SetEscapeFlag();
};


