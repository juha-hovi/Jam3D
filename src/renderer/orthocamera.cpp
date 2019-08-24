#include "orthocamera.h"

namespace Jam3D {

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, CameraBaseProperties properties)
    : Camera(properties)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_Near, m_Far);

    // Initialize vectors here since the camera is not meant to be rotated
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    m_CameraZ = glm::normalize(m_CameraZ);
    m_CameraX = glm::normalize(glm::cross(worldUp, m_CameraZ));
    // In case CameraZ points straight up (or straight down)
    if (m_Pitch == 90.0f)
    {
        m_CameraX = glm::vec3(1.0f, 0.0f, 0.0f);
        m_CameraY = -glm::cross(m_CameraZ, m_CameraX);        
    }
    else
        m_CameraY = glm::cross(m_CameraZ, m_CameraX);

    Update();
}

void OrthoCamera::Update()
{
	m_ViewMatrix = glm::lookAt(m_Position, m_Position - m_CameraZ, m_CameraY);
}

}