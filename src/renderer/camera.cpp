#include "camera.h"

namespace Jam3D {

Camera::Camera(float fov, float near, float far, Jam3D::Vec2 windowDim)
	: m_FoV(fov), m_Near(near), m_Far(far), m_WindowDimension(windowDim),
	m_FocusPoint({0.0f, 0.0f, 0.0f}), m_FocusPointDistance(1000.0f)
{
	m_ProjectionMatrix = glm::mat4(glm::perspective(glm::radians(m_FoV), m_WindowDimension.x / m_WindowDimension.y, m_Near, m_Far));
	m_ViewMatrix = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_FocusPointDistance)));
}

void Camera::Move(Jam3D::Vec3 moveVec)
{

}

void Camera::Zoom(float zoom)
{

}

void Camera::Update()
{
	m_ViewMatrix = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_FocusPointDistance)));
}

}