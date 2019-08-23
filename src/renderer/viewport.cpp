#include "viewport.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Jam3D {

Viewport::Viewport(Vec4<int> corners)
    : m_Corners(corners)
{
    m_Dimensions.x = m_Corners.x1 - m_Corners.x0;
    m_Dimensions.y = m_Corners.y1 - m_Corners.y0;    
}

void Viewport::Use()
{
    glViewport(m_Corners.x0, m_Corners.y0, m_Corners.x1, m_Corners.y1);
}

}