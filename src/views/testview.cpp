#include "testview.h"
#include "vec2.h"
#include "vec3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include <string>
#include <cmath>
#include <iostream>

namespace Jam3D {

TestView::TestView(std::shared_ptr<GLWindow> window)
    : m_Window(window), m_BoxCenter(0.0f, 0.0f, 0.0f), m_BoxDimensions(0.0f, 0.0f, 0.0f), m_BoxRotation(0.0f, 0.0f, 0.0f), 
    m_SphereCenter(0.0f, 0.0f, 0.0f), m_SphereRadius(100.0f), m_SphereSectorCount(10), m_SphereStackCount(10),
    m_ObjectRotation(Vec3(0.0f, 0.0f, 23.5f)), m_ObjectLocation(0.0f), m_ObjectDistance(500.0f),
    m_LightType(LightSource::POINT_LIGHT), m_LightPosition(Vec3(0.0f, 0.0f, 0.0f)), m_LightColor(Vec3(1.0f, 1.0f, 1.0f)),
    m_LightIntensity(1.0f)
{
    InitRendering();
    InitAxes();

    AddBox(Vec3(0.0f, -500.0f, 0.0f), Vec3(2000.0f, 10.0f, 2000.0f), Vec3(0.0f, 0.0f, 0.0f));
    AddSphere(100.0f, Vec3(sinf(m_ObjectLocation) * m_ObjectDistance, 0.0f, cosf(m_ObjectLocation) * m_ObjectDistance), 20, 20);
    AddLightSource(LightSource::POINT_LIGHT, Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), 1.0f);
}

void TestView::InitRendering()
{
    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);
    m_Layout->Push<float>(3);

    m_Shader = std::make_unique<Shader>("src/shaders/basic3d.shader");
    m_Shader->Bind();

    m_TextureBox = std::make_unique<Texture>("res/tex_test_full.png");
    m_TextureEarth = std::make_unique<Texture>("res/earth2048.bmp");
    m_TextureRGB = std::make_unique<Texture>("res/rgb.png");
    m_Shader->SetUniform1i("u_Texture", 0);

    float fov = 45.0f;
    float near = 1.0f;
    float far = 5000.0f;
    Vec2 windowDim({(float)m_Window->m_Width, (float)m_Window->m_Height});
    m_Camera = std::make_shared<Camera>(fov, near, far, windowDim, m_Window->m_Window);
}

void TestView::InitAxes()
{
    m_Renderer = std::make_unique<Renderer>();
    m_Axes = std::make_unique<Axes>();
    m_VAO_axes = std::make_unique<VertexArray>();
    m_VBO_axes = std::make_unique<VertexBuffer>(m_Axes->m_VertexData.data(), m_Axes->m_VertexData.size() * sizeof(float));
    m_Layout_axes = std::make_unique<VertexBufferLayout>();
    m_Layout_axes->Push<float>(3);
    m_Layout_axes->Push<float>(2);
    m_Layout_axes->Push<float>(3);
    m_VAO_axes->AddBuffer(*m_VBO_axes, *m_Layout_axes);
    m_IBO_axes = std::make_unique<IndexBuffer>(m_Axes->m_Indices.data(), m_Axes->m_Indices.size());
}

void TestView::AddBox(Vec3 center, Vec3 dimensions, Vec3 rotation)
{
    m_Boxes.push_back(Box(center, dimensions, rotation));
}

void TestView::DeleteBox(int index)
{
    m_Boxes.erase(m_Boxes.begin() + index);
}

void TestView::AddSphere(float radius, Vec3 center, int sectorCount, int stackCount)
{
    m_Spheres.push_back(Sphere(radius, center, sectorCount, stackCount));
}

void TestView::DeleteSphere(int index)
{
    m_Spheres.erase(m_Spheres.begin() + index);
}

void TestView::AddLightSource(unsigned int type, Vec3 position_or_direction, Vec3 color, float intensity)
{
    if (m_LightSources.size() <= 10)
    {
        m_LightSources.push_back(LightSource(type, position_or_direction, color, intensity));
    }
    else
    {
        std::cout << "[WARNING]: Already at max light sources!" << std::endl;
    }    
}

void TestView::DeleteLightSource(int index)
{
    m_LightSources.erase(m_LightSources.begin() + index);
}

void TestView::BufferBox(const Box& box)
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(box.m_VertexData.data(), box.m_VertexData.size() * sizeof(float));    
    m_VAO->AddBuffer(*m_VBO, *m_Layout);
    m_IBO = std::make_unique<IndexBuffer>(box.m_Indices.data(), box.m_Indices.size());
}

void TestView::BufferSphere(const Sphere& sphere)
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(sphere.m_VertexData.data(), sphere.m_VertexData.size() * sizeof(float));    
    m_VAO->AddBuffer(*m_VBO, *m_Layout);
    m_IBO = std::make_unique<IndexBuffer>(sphere.m_Indices.data(), sphere.m_Indices.size());
}

void TestView::SetLightSources()
{
    for (int i = 0; i < m_LightSources.size(); ++i)
    {
        std::string prefix = "u_LightSources[" + std::to_string(i) + "].";
        
        m_Shader->SetUniform3f((prefix + "lightPosition").c_str(), m_LightSources[i].m_Position.x, m_LightSources[i].m_Position.y, m_LightSources[i].m_Position.z);
        m_Shader->SetUniform3f((prefix + "lightColor").c_str(), m_LightSources[i].m_Color.r, m_LightSources[i].m_Color.g, m_LightSources[i].m_Color.b);
        m_Shader->SetUniform1f((prefix + "lightIntensity").c_str(), m_LightSources[i].m_Intensity);
    }

    m_Shader->SetUniform1i("u_LightSourceCount", m_LightSources.size());
}

void TestView::Render()
{
    m_Renderer->Clear();

    // 1st loop
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);    

    // 2nd loop
    m_Shader->Bind();

    m_Shader->SetUniformMat4f("u_View", m_Camera->m_ViewMatrix);
    m_Shader->SetUniformMat4f("u_Proj", m_Camera->m_ProjectionMatrix);

    SetLightSources();
    
    // 3rd loop
    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        BufferBox(m_Boxes[i]);

        glm::mat4 model(1.0f);
        glm::vec3 translation(m_Boxes[i].m_Center.x, m_Boxes[i].m_Center.y, m_Boxes[i].m_Center.z);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

        m_TextureBox->Bind();
        m_Shader->SetUniform1i("u_ApplyLighting", true);
        m_Shader->SetUniformMat4f("u_Model", model);

        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    }

    for (int i = 0; i < m_Spheres.size(); ++i)
    {
        float locationMult = 0.005f;
        m_ObjectLocation += locationMult;
        if (m_ObjectLocation > (2.0f * M_PI))
            m_ObjectLocation = 0.0f;

        float rotationMult = 0.8f;
        m_ObjectRotation += Vec3(0.0f, rotationMult, 0.0f);
        if (m_ObjectRotation.y >= 360.0f)
            m_ObjectRotation =  Vec3(0.0f, 0.0f, 23.5f);

        m_Spheres[0].m_Center = Vec3(sinf(m_ObjectLocation) * m_ObjectDistance, 0.0f, cosf(m_ObjectLocation) * m_ObjectDistance);
        m_Spheres[0].m_Rotation = m_ObjectRotation;

        BufferSphere(m_Spheres[i]);

        glm::mat4 model(1.0f);
        glm::vec3 translation(m_Spheres[i].m_Center.x, m_Spheres[i].m_Center.y, m_Spheres[i].m_Center.z);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

        m_TextureEarth->Bind();     
        m_Shader->SetUniform1i("u_ApplyLighting", true);         
        m_Shader->SetUniformMat4f("u_Model", model);

        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    }
    
    {
        glm::mat4 model(1.0f);
        m_Shader->SetUniform1i("u_ApplyLighting", false);
        m_TextureRGB->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Renderer->Draw(GL_LINES, *m_VAO_axes, *m_IBO_axes, *m_Shader);
    }
}

void TestView::RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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
                DeleteBox(i);
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
                DeleteSphere(i);
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
                DeleteLightSource(i);
        }
        ImGui::Text("================");
        for (int i = 0; i < m_LightSources.size(); ++i)
        {
            ImGui::SliderFloat3(("Pos " + std::to_string(i)).c_str(), &m_LightSources[i].m_Position.x, -500.0f, 500.0f);
        }    
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}