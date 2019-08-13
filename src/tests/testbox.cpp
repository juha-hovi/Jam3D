#include "testbox.h"
#include "vec2.h"
#include "vec3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

namespace Jam3D {

TestBox::TestBox(GLFWwindow* window)
    : m_Rotation(0.0f), m_Increment(0.5f), m_Window(window)
{
    m_Renderer = std::make_unique<Renderer>();

    // For drawing a box
    m_Box = std::make_unique<Box>(Vec3(-100.0f, -100.0f, -100.0f), Vec3(100.0f, 100.0f, 100.0f));
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(m_Box->m_Positions.data(), m_Box->m_PositionsSize * sizeof(float));
    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, *m_Layout);
    m_IBO = std::make_unique<IndexBuffer>(m_Box->m_Indices.data(), m_Box->m_IndicesSize);

    // For drawing axes
    m_Axes = std::make_unique<Axes>();
    m_VAO_axes = std::make_unique<VertexArray>();
    m_VBO_axes = std::make_unique<VertexBuffer>(m_Axes->m_Positions.data(), m_Axes->m_PositionsSize * sizeof(float));
    m_Layout_axes = std::make_unique<VertexBufferLayout>();
    m_Layout_axes->Push<float>(3);
    m_Layout_axes->Push<float>(2);
    m_VAO_axes->AddBuffer(*m_VBO_axes, *m_Layout_axes);
    m_IBO_axes = std::make_unique<IndexBuffer>(m_Axes->m_Indices.data(), m_Axes->m_IndicesSize);

    m_Shader = std::make_unique<Shader>("src/shaders/basic3d.shader");
    m_Shader->Bind();

    m_Texture = std::make_unique<Texture>("src/resources/tex_test_full.png");
    m_Shader->SetUniform1i("u_Texture", 0);

    float fov = 45.0f;
    float near = 1.0f;
    float far = 5000.0f;
    Vec2 windowDim({960.0f, 540.0f});
    m_Camera = std::make_unique<Camera>(fov, near, far, windowDim, m_Window);
}

TestBox::~TestBox()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void TestBox::Update()
{
    m_Box->Update();
    m_VBO->UpdateBuffer(0, m_Box->m_PositionsSize * sizeof(float), m_Box->m_Positions.data());
    m_Camera->Update();
}

void TestBox::Render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Update();
    m_Renderer->Clear();
    
    glm::mat4 model(1.0f);
    glm::vec3 translation(m_Box->m_Center.x, m_Box->m_Center.y, m_Box->m_Center.z);
    model = glm::translate(model, translation);
    model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.5f, 1.0f, 0.0f));
    model = glm::translate(model, -translation);
    glm::mat4 mvp = m_Camera->m_ProjectionMatrix * m_Camera->m_ViewMatrix * model;
    m_Texture->Bind();
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    m_Renderer->Draw(GL_LINES, *m_VAO_axes, *m_IBO_axes, *m_Shader);

    if (m_Rotation >= 360.0f)
            m_Rotation = 0.0f;
    //m_Rotation += m_Increment;
}

void TestBox::RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("OpenGL Project");

        ImGui::SliderFloat("Corner 0: x", &m_Box->m_Corner0.x, -500.0f, 500.0f);
        ImGui::SliderFloat("Corner 0: y", &m_Box->m_Corner0.y, -500.0f, 500.0f);
        ImGui::SliderFloat("Corner 0: z", &m_Box->m_Corner0.z, -500.0f, 500.0f);
        ImGui::SliderFloat("Corner 1: x", &m_Box->m_Corner1.x, -500.0f, 500.0f);
        ImGui::SliderFloat("Corner 1: y", &m_Box->m_Corner1.y, -500.0f, 500.0f);
        ImGui::SliderFloat("Corner 1: z", &m_Box->m_Corner1.z, -500.0f, 500.0f);

        ImGui::SliderFloat("Zoom", &m_Camera->m_FocusPointDistance, 0.0f, 2000.0f);
        ImGui::SliderFloat("Rotation: x", &m_Camera->m_Rotation.x, 0.0f, 90.0f);
        ImGui::SliderFloat("Rotation: y", &m_Camera->m_Rotation.y, 0.0f, 90.0f);
        ImGui::SliderFloat("Rotation: z", &m_Camera->m_Rotation.z, 0.0f, 90.0f);
        ImGui::SliderFloat("FocusPoint: x", &m_Camera->m_FocusPoint.x, -100.0f, 100.0f);
        ImGui::SliderFloat("FocusPoint: y", &m_Camera->m_FocusPoint.y, -100.0f, 100.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}