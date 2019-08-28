#include "viewport.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Jam3D {

Viewport::Viewport(Vec4<int> corners)
    : m_Corners(corners)
{

}

void Viewport::Use()
{
    glViewport(m_Corners.x0, m_Corners.y0, m_Corners.x1, m_Corners.y1);
}

}