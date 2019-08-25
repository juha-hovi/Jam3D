#include "objectcreationview.h"
#include "vec4.h"
#include "mathhelper.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

namespace Jam3D {

ObjectCreationView::ObjectCreationView(std::shared_ptr<GLWindow> window) 
    : View(window), m_UpperLeftViewportIndex(-1), m_UpperRightViewportIndex(-1), m_LowerLeftViewportIndex(-1),
    m_LowerRightViewportIndex(-1), m_ShadowViewportIndex(-1), m_MouseRightPressedUpperLeft(false),
    m_SelectedLocation(glm::vec3(0.0f, 0.0f, 0.0f)), m_DrawPlanes(true)
{
    InitViewports();
    InitCameras();
}

void ObjectCreationView::InitViewports()
{
    int divider = 10;
    m_UpperLeftViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(0, m_Window->m_Height / 2 + divider, m_Window->m_Width / 2 - divider, m_Window->m_Height / 2 - divider));
    m_UpperRightViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(m_Window->m_Width / 2 + divider, m_Window->m_Height / 2 + divider, m_Window->m_Width / 2 - divider, m_Window->m_Height / 2 - divider));
    m_LowerLeftViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(0, 0, m_Window->m_Width / 2 - divider, m_Window->m_Height / 2 - divider));
    m_LowerRightViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(m_Window->m_Width / 2 + divider, 0, m_Window->m_Width / 2 - divider, m_Window->m_Height / 2 - divider));
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

    float left = -1200;
    float right = 1200;
    float bottom = -675;
    float top = 675;
    properties.pitch = 90.0f;
    properties.position = glm::vec3(0.0f, 2000.0f, 0.0f);
    m_UpperRightCamera = std::make_shared<OrthoCamera>(left, right, bottom, top, properties);

    properties.pitch = 0.0f;
    properties.position = glm::vec3(0.0f, 0.0f, -2000.0f);
    m_LowerLeftCamera = std::make_shared<OrthoCamera>(left, right, bottom, top, properties);

    properties.position = glm::vec3(2000.0f, 0.0f, 0.0f);
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
    RenderScene(*m_UpperLeftCamera, true);
    RenderMisc(*m_UpperLeftCamera, true, false, false, false);

    // Render xz
    m_Viewports[m_UpperRightViewportIndex].Use();
    RenderScene(*m_UpperRightCamera, false);
    glDisable(GL_CULL_FACE); 
    RenderMisc(*m_UpperRightCamera, true, m_DrawPlanes, false, false);
    glEnable(GL_CULL_FACE); 

    // Render xy
    m_Viewports[m_LowerLeftViewportIndex].Use();
    RenderScene(*m_LowerLeftCamera, false);
    glDisable(GL_CULL_FACE); 
    RenderMisc(*m_LowerLeftCamera, true, false, m_DrawPlanes, false);
    glEnable(GL_CULL_FACE); 

    // Render yz
    m_Viewports[m_LowerRightViewportIndex].Use();
    RenderScene(*m_LowerRightCamera, false);
    glDisable(GL_CULL_FACE); 
    RenderMisc(*m_LowerRightCamera, true, false, false, m_DrawPlanes);
    glEnable(GL_CULL_FACE); 

    RenderImGui();
}

void ObjectCreationView::RenderImGui()
{
    {
        ImGui::Begin("Grid");
        if (ImGui::Button("On/Off"))
            m_DrawPlanes = !m_DrawPlanes;
        ImGui::End();
    }
}

void ObjectCreationView::CursorPosCallback(double xPos, double yPos)
{
    if (m_MouseRightPressedUpperLeft)
        m_UpperLeftCamera->CursorPosCallback(xPos, yPos);

    // Upper-left: Normal
    else if (IsInViewport(xPos, yPos, m_Viewports[m_UpperLeftViewportIndex]))
        m_UpperLeftCamera->CursorPosCallback(xPos, yPos);
}

void ObjectCreationView::KeyCallback(int key, int scancode, int action, int mods)
{
    double xPos, yPos;
    glfwGetCursorPos(m_Window->m_Window, &xPos, &yPos);

    // Upper-left: Normal
    if (IsInViewport(xPos, yPos, m_Viewports[m_UpperLeftViewportIndex]))
        m_UpperLeftCamera->KeyCallback(key, scancode, action, mods);
}

void ObjectCreationView::MouseButtonCallback(int button, int action, int mods)
{
    double xPos, yPos;
    glfwGetCursorPos(m_Window->m_Window, &xPos, &yPos);

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        m_UpperLeftCamera->MouseButtonCallback(button, action, mods);
        m_MouseRightPressedUpperLeft = false;
    }

    // Upper-left: Normal
    if (IsInViewport(xPos, yPos, m_Viewports[m_UpperLeftViewportIndex]))
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            m_MouseRightPressedUpperLeft = true;
        m_UpperLeftCamera->MouseButtonCallback(button, action, mods);
    }

    // Upper-Right: XZ
    else if (IsInViewport(xPos, yPos, m_Viewports[m_UpperRightViewportIndex]))
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glm::vec4 viewport = Jam3D::MathHelper::Vec4ToGlm(m_Viewports[m_UpperRightViewportIndex].m_Corners);
            glm::vec3 worldCoords = glm::unProject(glm::vec3(xPos, m_Window->m_Height - yPos, 1.0f), m_UpperRightCamera->m_ViewMatrix, m_UpperRightCamera->m_ProjectionMatrix, viewport);
            m_SelectedLocation.x = -worldCoords.x; // Flip to fix coordinate system handedness
            m_SelectedLocation.z = worldCoords.z;
        }
    }

    // Lower-Left: XY
    else if (IsInViewport(xPos, yPos, m_Viewports[m_LowerLeftViewportIndex]))
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glm::vec4 viewport = Jam3D::MathHelper::Vec4ToGlm(m_Viewports[m_LowerLeftViewportIndex].m_Corners);
            glm::vec3 worldCoords = glm::unProject(glm::vec3(xPos, m_Window->m_Height - yPos, 1.0f), m_LowerLeftCamera->m_ViewMatrix, m_LowerLeftCamera->m_ProjectionMatrix, viewport);
            m_SelectedLocation.x = -worldCoords.x; // Flip to fix coordinate system handedness
            m_SelectedLocation.y = worldCoords.y;
        }
    }

    // Lower-Right: YZ
    else if (IsInViewport(xPos, yPos, m_Viewports[m_LowerRightViewportIndex]))
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glm::vec4 viewport = Jam3D::MathHelper::Vec4ToGlm(m_Viewports[m_LowerRightViewportIndex].m_Corners);
            glm::vec3 worldCoords = glm::unProject(glm::vec3(xPos, m_Window->m_Height - yPos, 1.0f), m_LowerRightCamera->m_ViewMatrix, m_LowerRightCamera->m_ProjectionMatrix, viewport);
            m_SelectedLocation.z = worldCoords.z;
            m_SelectedLocation.y = worldCoords.y;
        }
    }
}

void ObjectCreationView::ScrollCallback(double yOffset)
{
    double xPos, yPos;
    glfwGetCursorPos(m_Window->m_Window, &xPos, &yPos);

    // Upper-left: Normal
    if (IsInViewport(xPos, yPos, m_Viewports[m_UpperLeftViewportIndex]))
        m_UpperLeftCamera->ScrollCallback(yOffset);
}

}