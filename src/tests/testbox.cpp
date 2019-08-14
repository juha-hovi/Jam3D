#include "testbox.h"
#include "vec2.h"
#include "vec3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include <string>

namespace Jam3D {

TestBox::TestBox(GLFWwindow* window)
    : m_Rotation(0.0f), m_Increment(0.5f), m_Window(window), m_PositionsSize(0),
    m_IndicesSize(0), m_Corner0(0.0f, 0.0f, 0.0f), m_Corner1(0.0f, 0.0f, 0.0f)
{
    InitRendering();
    InitAxes();

    AddBox(Vec3(-300.0f, -300.0f, -300.0f), Vec3(-100.0f, -100.0f, -100.0f));
    AddBox(Vec3(100.0f, 100.0f, 100.0f), Vec3(300.0f, 300.0f, 300.0f));
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
    Vec2 windowDim({960.0f, 540.0f});
    m_Camera = std::make_shared<Camera>(fov, near, far, windowDim, m_Window);
}

void TestBox::InitAxes()
{
    m_Renderer = std::make_unique<Renderer>();
    m_Axes = std::make_unique<Axes>();
    m_VAO_axes = std::make_unique<VertexArray>();
    m_VBO_axes = std::make_unique<VertexBuffer>(m_Axes->m_Positions.data(), m_Axes->m_PositionsSize * sizeof(float));
    m_Layout_axes = std::make_unique<VertexBufferLayout>();
    m_Layout_axes->Push<float>(3);
    m_Layout_axes->Push<float>(2);
    m_VAO_axes->AddBuffer(*m_VBO_axes, *m_Layout_axes);
    m_IBO_axes = std::make_unique<IndexBuffer>(m_Axes->m_Indices.data(), m_Axes->m_IndicesSize);
}

void TestBox::AddBox(Vec3 corner0, Vec3 corner1)
{
    m_Boxes.push_back(Box(corner0, corner1));
    UpdateNewBoxes();
}

void TestBox::UpdateNewBoxes()
{
    m_Positions.clear();
    m_Indices.clear();
    m_PositionsSize = 0;
    m_IndicesSize = 0;

    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        m_Boxes[i].Update();
        m_PositionsSize += m_Boxes[i].m_PositionsSize;
        m_IndicesSize += m_Boxes[i].m_IndicesSize;
        for (int j = 0; j < m_Boxes[i].m_PositionsSize; ++j)
        {
            m_Positions.push_back(m_Boxes[i].m_Positions[j]);
        }
        for (int j = 0; j < m_Boxes[i].m_IndicesSize; ++j)
        {
            m_Indices.push_back(m_Boxes[i].m_Indices[j] + (i * m_Boxes[i].m_Vertices));
        }
    }

    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(m_Positions.data(), m_PositionsSize * sizeof(float));    
    m_VAO->AddBuffer(*m_VBO, *m_Layout);
    m_IBO = std::make_unique<IndexBuffer>(m_Indices.data(), m_IndicesSize);
}

void TestBox::UpdateExistingBoxes()
{
    m_VBO->UpdateBuffer(0, m_PositionsSize * sizeof(float), m_Positions.data());
}

void TestBox::Render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    UpdateExistingBoxes();
    m_Renderer->Clear();
    
    glm::mat4 model(1.0f);
    //glm::vec3 translation(m_Box->m_Center.x, m_Box->m_Center.y, m_Box->m_Center.z);
    //model = glm::translate(model, translation);
    //model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.5f, 1.0f, 0.0f));
    //model = glm::translate(model, -translation);
    glm::mat4 mvp = m_Camera->m_ProjectionMatrix * m_Camera->m_ViewMatrix * model;
    m_Texture->Bind();
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    m_Renderer->Draw(GL_LINES, *m_VAO_axes, *m_IBO_axes, *m_Shader);
}

void TestBox::RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Jam3D");

        ImGui::InputFloat("c0: x", &m_Corner0.x, 0.1f, 1.0f);
        ImGui::InputFloat("c0: y", &m_Corner0.y, 0.1f, 1.0f);
        ImGui::InputFloat("c0: z", &m_Corner0.z, 0.1f, 1.0f);
        ImGui::InputFloat("c1: x", &m_Corner1.x, 0.1f, 1.0f);
        ImGui::InputFloat("c1: y", &m_Corner1.y, 0.1f, 1.0f);
        ImGui::InputFloat("c1: z", &m_Corner1.z, 0.1f, 1.0f);
        if (ImGui::Button("Add box"))
        {
            AddBox(m_Corner0, m_Corner1);
        }

        if (ImGui::Button("Delete box"))
        {
            ImGui::OpenPopup("Box list")
        }
        if (ImGui::BeginPopup("Box list"))
        {
            for (int i = 0; i < m_Boxes.size(); ++i)
            {
                if (ImGui::Button(std::to_string(i)))
                    m_Boxes.erase(i);
            }
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}