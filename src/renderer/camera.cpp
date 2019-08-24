#include "camera.h"

namespace Jam3D {

Camera::Camera(float near, float far, Jam3D::Vec2<float> windowDim, std::shared_ptr<GLWindow> window)
    : m_Near(near), m_Far(far), m_WindowDimension(windowDim), m_CameraX(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_CameraY(glm::vec3(0.0f, 0.0f, 0.0f))
{
    m_Window = window;
}

}