#include "PCamera.h"
#include "glm/gtc/matrix_transform.hpp"

PCamera::PCamera(float angle, float ratio, float near, float far)
	: m_ProjectionMatrix(glm::perspective(glm::radians(angle), ratio, near, far))
{
}

void PCamera::Update()
{
	m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3(0, 1, 0));
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
