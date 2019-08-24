#include "camera.h"

namespace Jam3D {

Camera::Camera(float near, float far, Jam3D::Vec2<float> windowDim, std::shared_ptr<GLWindow> window)
    : m_Near(near), m_Far(far), m_WindowDimension(windowDim)
{
    m_Window = window;
}

}