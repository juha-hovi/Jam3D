#include "camera.h"

namespace Jam3D {

Camera::Camera(CameraBaseProperties properties)
    : m_Near(properties.near), m_Far(properties.far), m_WindowDimension(properties.windowDim), 
    m_Position(properties.position), m_Pitch(properties.pitch), m_Yaw(properties.yaw), 
    m_CameraX(glm::vec3(0.0f, 0.0f, 0.0f)),	m_CameraY(glm::vec3(0.0f, 0.0f, 0.0f))
{
    m_Window = properties.window;
    m_CameraZ = glm::normalize(m_Position - properties.target);
}

}