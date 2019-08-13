#include "camera.h"

namespace Jam3D {

Camera::Camera(float fov, float near, float far, Jam3D::Vec2 windowDim, GLFWwindow* window)
	: m_Window(window), m_FoV(fov), m_Near(near), m_Far(far), m_WindowDimension(windowDim),
	m_FocusPoint({0.0f, 0.0f}), m_FocusPointDistance(1000.0f), m_Rotation({0.0f, 0.0f, 0.0f}),
	m_RotationOld({0.0f, 0.0f, 0.0f}), m_RotationSensitivity(0.5), m_CursorOriginX(0.0), m_CursorOriginY(0.0)
{
	m_ProjectionMatrix = glm::mat4(glm::perspective(glm::radians(m_FoV), m_WindowDimension.x / m_WindowDimension.y, m_Near, m_Far));
	m_ViewMatrix = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_FocusPointDistance)));
}

void Camera::Update()
{
	m_ViewMatrix = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_FocusPointDistance)));

	m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(m_FocusPoint.x, m_FocusPoint.y, 0.0f));

	m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::CursorPosCallback(double xPos, double yPos)
{
	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
	    m_Rotation.y = m_RotationOld.y + m_RotationSensitivity * (xPos - m_CursorOriginX);
		m_Rotation.x = m_RotationOld.x + m_RotationSensitivity * (yPos - m_CursorOriginY);
	}
	else
	{
		m_CursorOriginX = xPos;
		m_CursorOriginY = yPos;
		m_RotationOld.y = m_Rotation.y;
		m_RotationOld.x = m_Rotation.x;
	}

	// Prevent flipping upside down
	if (m_Rotation.x > 90)
		m_Rotation.x = 90;
	if (m_Rotation.x < -90)
		m_Rotation.x = -90;
}

}