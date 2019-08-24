#include "objectcreationview.h"
#include "vec4.h"

namespace Jam3D {

ObjectCreationView::ObjectCreationView(std::shared_ptr<GLWindow> window) 
    : View(window)
{
    InitViewports();
    InitCameras();
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
    m_ShadowViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Viewport(Jam3D::Vec4<int>(0, 0, m_ShadowWidth, m_ShadowHeight)));
}

void ObjectCreationView::InitCameras()
{
    Camera::CameraBaseProperties properties;
    float fov = 45.0f;
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    properties.near = 1.0f;
    properties.far = 5000.0f;
    properties.position = glm::vec3(1000.0f, 1000.0f, -1000.0f);
    properties.target = glm::vec3(0.0f, 0.0f, 0.0f);
    properties.pitch = 35.0f;
    properties.yaw = -45.0f;
    properties.windowDim = Jam3D::Vec2<float>({(float)m_Window->m_Width, (float)m_Window->m_Height});
    properties.window = m_Window;
    m_UpperLeftCamera = std::make_shared<PerspectiveCamera>(fov, target, properties);

    float left = -1000;
    float right = 1000;
    float bottom = -1000;
    float top = 1000;
    properties.position = glm::vec3(0.0f, 1000.0f, 0.0f);
    m_UpperRightCamera = std::make_shared<OrthoCamera>(left, right, bottom, top, properties);

    properties.position = glm::vec3(0.0f, 0.0f, 1000.0f);
    m_LowerLeftCamera = std::make_shared<OrthoCamera>(left, right, bottom, top, properties);

    properties.position = glm::vec3(1000.0f, 0.0f, 0.0f);
    m_LowerRightCamera = std::make_shared<OrthoCamera>(left, right, bottom, top, properties);
}

void ObjectCreationView::Render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  

    UpdateModelMats();
    m_UpperLeftCamera->Update();
    m_UpperRightCamera->Update();
    m_LowerLeftCamera->Update();
    m_LowerRightCamera->Update();

    // Render shadows
    m_Viewports[m_ShadowViewportIndex].Use();
    RenderPointShadow();

    m_Renderer->Clear();

    // Render overall window
    m_Viewports[m_UpperLeftViewportIndex].Use();
    RenderScene(*m_UpperLeftCamera);

    // Render xz
    m_Viewports[m_UpperRightViewportIndex].Use();
    RenderScene(*m_UpperRightCamera);

    // Render xy
    m_Viewports[m_LowerLeftViewportIndex].Use();
    RenderScene(*m_LowerLeftCamera);

    // Render yz
    m_Viewports[m_LowerRightViewportIndex].Use();
    RenderScene(*m_LowerRightCamera);

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