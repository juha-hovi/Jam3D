#include "objectcreationview.h"
#include "vec4.h"

namespace Jam3D {

ObjectCreationView::ObjectCreationView(std::shared_ptr<GLWindow> window) : View(window)
{
    InitViewports();
}

void ObjectCreationView::InitViewports()
{
    m_UpperLeftViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(0, m_Window->m_Height / 2, m_Window->m_Width / 2, m_Window->m_Height / 2));
    m_UpperRightViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(m_Window->m_Width / 2, m_Window->m_Height / 2, m_Window->m_Width / 2, m_Window->m_Height / 2));
    m_LowerLeftViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(0, 0, m_Window->m_Width / 2, m_Window->m_Height / 2));
    m_LowerRightViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(m_Window->m_Width / 2, 0, m_Window->m_Width / 2, m_Window->m_Height / 2));
}

void ObjectCreationView::Render()
{
    m_Renderer->Clear();
}

void ObjectCreationView::CursorPosCallback(double xPos, double yPos)
{
    
}

void ObjectCreationView::KeyCallback(int key, int scancode, int action, int mods)
{

}

void ObjectCreationView::MouseButtonCallback(int button, int action, int mods)
{

}

void ObjectCreationView::ScrollCallback(double yOffset)
{

}

}