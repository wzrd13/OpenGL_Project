#pragma once

#include "glm/glm.hpp"

class PCamera
{
public:
	PCamera(float angle, float ratio, float near, float far);

	inline const glm::vec3 GetPosition() const { return m_Position; }
	inline const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	inline void SetPosition(glm::vec3 position) { m_Position = position; Update(); }
	inline void SetLookAt(glm::vec3 lookat) { m_LookAt = lookat; Update(); }
	inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; Update(); }

private:
	void Update();

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_LookAt;
};