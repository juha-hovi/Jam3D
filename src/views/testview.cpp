#include "testview.h"
#include "vec2.h"
#include "vec3.h"
#include "log.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include <string>
#include <cmath>

namespace Jam3D {

TestView::TestView(std::shared_ptr<GLWindow> window) 
    : View(window),
    m_BoxCenter(0.0f, 0.0f, 0.0f), m_BoxDimensions(0.0f, 0.0f, 0.0f), m_BoxRotation(0.0f, 0.0f, 0.0f), 
    m_SphereCenter(0.0f, 0.0f, 0.0f), m_SphereRadius(100.0f), m_SphereSectorCount(10), m_SphereStackCount(10),
    m_LightType(LightSource::POINT_LIGHT), m_LightPosition(Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f)), m_LightColor(Jam3D::Vec3<float>(1.0f, 1.0f, 1.0f)),
    m_LightIntensity(1.0f), m_NormalViewportIndex(-1), m_ShadowViewportIndex(-1)
{
    AddBox(Jam3D::Vec3<float>(0.0f, -200.0f, 0.0f), Jam3D::Vec3<float>(70.0f, 50.0f, 90.0f), Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f));
    AddBox(Jam3D::Vec3<float>(0.0f, -500.0f, 0.0f), Jam3D::Vec3<float>(1900.0f, 100.0f, 1900.0f), Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f));
    AddSphere(100.0f, Jam3D::Vec3<float>(500.0f, -300.0f, 0.0f), 20, 20);
    AddLightSource(LightSource::POINT_LIGHT, Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f), Jam3D::Vec3<float>(1.0f, 1.0f, 1.0f), 1.0f);

    InitViewports();
    InitCameras();
}

void TestView::InitViewports()
{
    m_NormalViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Viewport(Jam3D::Vec4<int>(0, 0, m_Window->m_Width, m_Window->m_Height)));

    m_ShadowViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Viewport(Jam3D::Vec4<int>(0, 0, m_ShadowWidth, m_ShadowHeight)));
}

void TestView::InitCameras()
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
    m_Camera = std::make_shared<PerspectiveCamera>(fov, target, properties);
}

void TestView::Render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_Camera->Update();
    UpdateModelMats();

    m_Viewports[m_ShadowViewportIndex].Use();
    RenderPointShadow();

    m_Renderer->Clear();

    m_Viewports[m_NormalViewportIndex].Use();
    RenderScene(*m_Camera, true);
    RenderMisc(*m_Camera);

    RenderImGui();
}

void TestView::RenderImGui()
{
    {
        ImGui::Begin("Boxes");
        ImGui::InputFloat3("Center", &m_BoxCenter.x, 1.0f, 1.0f);
        ImGui::InputFloat3("Dim", &m_BoxDimensions.x, 1.0f, 1.0f);
        ImGui::InputFloat3("Rot", &m_BoxRotation.x, 1.0f, 1.0f);
        if (ImGui::Button("Add"))
        {
            AddBox(m_BoxCenter, m_BoxDimensions, m_BoxRotation);
        }
        
        ImGui::Text("================");
        for (int i = 0; i < m_Boxes.size(); ++i)
        {
            if (ImGui::Button(("Delete " + std::to_string(i)).c_str()))
                m_Boxes.erase(m_Boxes.begin() + i);
        }

        ImGui::Text("================");
        for (int i = 0; i < m_Boxes.size(); ++i)
        {
            ImGui::SliderFloat3(("Rot " + std::to_string(i)).c_str(), &m_Boxes[i].m_Rotation.x, 0.0f, 360.0f);
        }

        ImGui::Text("================");
        for (int i = 0; i < m_Boxes.size(); ++i)
        {
            ImGui::SliderFloat3(("Pos " + std::to_string(i)).c_str(), &m_Boxes[i].m_Center.x, -500.0f, 500.0f);
        }
        ImGui::End();
    }

    {
        ImGui::Begin("Spheres");
        ImGui::InputFloat3("Center", &m_SphereCenter.x, 1.0f, 1.0f);
        ImGui::InputFloat("Radius", &m_SphereRadius, 1.0f, 1.0f);
        ImGui::InputInt("Sectors", &m_SphereSectorCount);
        ImGui::InputInt("Stacks", &m_SphereStackCount);
        if (ImGui::Button("Add"))
        {
            AddSphere(m_SphereRadius, m_SphereCenter, m_SphereSectorCount, m_SphereStackCount);
        }
        ImGui::Text("================");
        for (int i = 0; i < m_Spheres.size(); ++i)
        {
            if (ImGui::Button(("Delete " + std::to_string(i)).c_str()))
                m_Spheres.erase(m_Spheres.begin() + i);
        }

        ImGui::Text("================");
        for (int i = 0; i < m_Spheres.size(); ++i)
        {
            ImGui::SliderFloat3(("Rot " + std::to_string(i)).c_str(), &m_Spheres[i].m_Rotation.x, 0.0f, 360.0f);
        }

        ImGui::Text("================");
        for (int i = 0; i < m_Spheres.size(); ++i)
        {
            ImGui::SliderFloat3(("Pos " + std::to_string(i)).c_str(), &m_Spheres[i].m_Center.x, -500.0f, 500.0f);
        }
        ImGui::End();
    }

    {
        ImGui::Begin("LightSources");
        ImGui::InputFloat3("Position", &m_LightPosition.x, 1.0f, 1.0f);
        ImGui::InputFloat3("Color", &m_LightColor.r, 1.0f, 1.0f);
        ImGui::InputFloat("Intensity", &m_LightIntensity);
        if (ImGui::Button("Add"))
        {
            AddLightSource(LightSource::POINT_LIGHT, m_LightPosition, m_LightColor, m_LightIntensity);
        }
        ImGui::Text("================");
        for (int i = 0; i < m_LightSources.size(); ++i)
        {
            if (ImGui::Button(("Delete " + std::to_string(i)).c_str()))
                m_LightSources.erase(m_LightSources.begin() + i);;
        }
        ImGui::Text("================");
        for (int i = 0; i < m_LightSources.size(); ++i)
        {
            ImGui::SliderFloat3(("Pos " + std::to_string(i)).c_str(), &m_LightSources[i].m_Position.x, -500.0f, 500.0f);
        }    
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

void TestView::CursorPosCallback(double xPos, double yPos)
{
    m_Camera->CursorPosCallback(xPos, yPos);
}

void TestView::KeyCallback(int key, int scancode, int action, int mods)
{
    m_Camera->KeyCallback(key, scancode, action, mods);
}

void TestView::MouseButtonCallback(int button, int action, int mods)
{
    m_Camera->MouseButtonCallback(button, action, mods);
}

void TestView::ScrollCallback(double yOffset)
{
    m_Camera->ScrollCallback(yOffset);
}


}