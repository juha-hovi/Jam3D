#include "testbox.h"
#include "vec2.h"
#include "vec3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include <string>

namespace Jam3D {

TestBox::TestBox(std::shared_ptr<GLWindow> window)
    : m_Window(window), m_BoxCenter(0.0f, 0.0f, 0.0f), m_BoxDimensions(0.0f, 0.0f, 0.0f), m_BoxRotation(0.0f, 0.0f, 0.0f), 
    m_SphereCenter(0.0f, 0.0f, 0.0f), m_SphereRadius(100.0f), m_SphereSectorCount(10), m_SphereStackCount(10)
{
    InitRendering();
    InitAxes();

    AddBox(Vec3(200.0f, -200.0f, 0.0f), Vec3(200.0f, 200.0f, 200.0f), Vec3(0.0f, 0.0f, 0.0f));
    AddSphere(100.0f, Vec3(-200.0f, -200.0f, 0.0f), 20, 20);
}

void TestBox::InitRendering()
{
    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);

    m_Shader = std::make_unique<Shader>("src/shaders/basic3d.shader");
    m_Shader->Bind();

    m_Texture = std::make_unique<Texture>("src/resources/tex_test_full.png");
    m_Shader->SetUniform1i("u_Texture", 0);

    float fov = 45.0f;
    float near = 1.0f;
    float far = 5000.0f;
    Vec2 windowDim({(float)m_Window->m_Width, (float)m_Window->m_Height});
    m_Camera = std::make_shared<Camera>(fov, near, far, windowDim, m_Window->m_Window);
}

void TestBox::InitAxes()
{
    m_Renderer = std::make_unique<Renderer>();
    m_Axes = std::make_unique<Axes>();
    m_VAO_axes = std::make_unique<VertexArray>();
    m_VBO_axes = std::make_unique<VertexBuffer>(m_Axes->m_Positions.data(), m_Axes->m_Positions.size() * sizeof(float));
    m_Layout_axes = std::make_unique<VertexBufferLayout>();
    m_Layout_axes->Push<float>(3);
    m_Layout_axes->Push<float>(2);
    m_VAO_axes->AddBuffer(*m_VBO_axes, *m_Layout_axes);
    m_IBO_axes = std::make_unique<IndexBuffer>(m_Axes->m_Indices.data(), m_Axes->m_Indices.size());
}

void TestBox::AddBox(Vec3 center, Vec3 dimensions, Vec3 rotation)
{
    m_Boxes.push_back(Box(center, dimensions, rotation));
}

void TestBox::DeleteBox(int index)
{
    m_Boxes.erase(m_Boxes.begin() + index);
}

void TestBox::AddSphere(float radius, Vec3 center, int sectorCount, int stackCount)
{
    m_Spheres.push_back(Sphere(radius, center, sectorCount, stackCount));
}

void TestBox::DeleteSphere(int index)
{
    m_Spheres.erase(m_Spheres.begin() + index);
}

void TestBox::BufferBox(const Box& box)
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(box.m_Positions.data(), box.m_Positions.size() * sizeof(float));    
    m_VAO->AddBuffer(*m_VBO, *m_Layout);
    m_IBO = std::make_unique<IndexBuffer>(box.m_Indices.data(), box.m_Indices.size());
}

void TestBox::BufferSphere(const Sphere& sphere)
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(sphere.m_Positions.data(), sphere.m_Positions.size() * sizeof(float));    
    m_VAO->AddBuffer(*m_VBO, *m_Layout);
    m_IBO = std::make_unique<IndexBuffer>(sphere.m_Indices.data(), sphere.m_Indices.size());
}

void TestBox::Render()
{
    m_Renderer->Clear();

    // 1st loop
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);    

    // 2nd loop
    m_Texture->Bind();
    m_Shader->Bind();
    
    // 3rd loop
    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        BufferBox(m_Boxes[i]);

        glm::mat4 model(1.0f);
        glm::vec3 translation(m_Boxes[i].m_Center.x, m_Boxes[i].m_Center.y, m_Boxes[i].m_Center.z);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::translate(model, -translation);
        glm::mat4 mvp = m_Camera->m_ProjectionMatrix * m_Camera->m_ViewMatrix * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    }

    for (int i = 0; i < m_Spheres.size(); ++i)
    {
        BufferSphere(m_Spheres[i]);

        glm::mat4 model(1.0f);
        glm::vec3 translation(m_Spheres[i].m_Center.x, m_Spheres[i].m_Center.y, m_Spheres[i].m_Center.z);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 mvp = m_Camera->m_ProjectionMatrix * m_Camera->m_ViewMatrix * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    }
    
    {
        glm::mat4 model(1.0f);
        glm::mat4 mvp = m_Camera->m_ProjectionMatrix * m_Camera->m_ViewMatrix * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Renderer->Draw(GL_LINES, *m_VAO_axes, *m_IBO_axes, *m_Shader);
    }
}

void TestBox::RenderImGui()
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
            ImGui::SliderFloat3(("Box " + std::to_string(i)).c_str(), &m_Boxes[i].m_Rotation.x, 0.0f, 360.0f);
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
            ImGui::SliderFloat3(("Sphere " + std::to_string(i)).c_str(), &m_Spheres[i].m_Rotation.x, 0.0f, 360.0f);
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}