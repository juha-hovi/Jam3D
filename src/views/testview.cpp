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
    m_ObjectRotation(0.0f), m_ObjectLocation(0.0f), m_ObjectDistance(500.0f),
    m_LightType(LightSource::POINT_LIGHT), m_LightPosition(Vec3(0.0f, 0.0f, 0.0f)), m_LightColor(Vec3(1.0f, 1.0f, 1.0f)),
    m_LightIntensity(1.0f), m_ShadowNearPlane(1.0f), m_ShadowFarPlane(5000), m_ShadowProjectionMatrix(glm::mat4(1.0f))
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  

    AddBox(Vec3(0.0f, -200.0f, 0.0f), Vec3(70.0f, 50.0f, 90.0f), Vec3(0.0f, 0.0f, 0.0f));
    AddBox(Vec3(0.0f, -500.0f, 0.0f), Vec3(2000.0f, 10.0f, 2000.0f), Vec3(0.0f, 0.0f, 0.0f));
    AddSphere(100.0f, Vec3(std::sin(m_ObjectLocation) * m_ObjectDistance, -300.0f, std::cos(m_ObjectLocation) * m_ObjectDistance), 20, 20);
    AddLightSource(LightSource::POINT_LIGHT, Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), 1.0f);

    InitRendering();
    InitAxes();
    InitPointShadow();
}

void TestView::InitRendering()
{
    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);
    m_Layout->Push<float>(3);

    m_Shader = std::make_unique<Shader>("src/shaders/basic3d.shader", Shader::VERTEX_FRAGMENT);
    m_Shader->Bind();

    m_TextureBox = std::make_unique<Texture2D>("res/tex_test_full.png");
    m_TextureEarth = std::make_unique<Texture2D>("res/earth2048.bmp");
    m_TextureRGB = std::make_unique<Texture2D>("res/rgb.png");
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

void TestView::InitPointShadow()
{
    m_FrameBuffer = std::make_unique<FrameBuffer>();
    m_TextureShadow = std::make_unique<TextureCubeMap>(m_ShadowWidth, m_ShadowHeight);
    m_FrameBuffer->AttachTexture(m_TextureShadow->GetRendererID());

    m_ShadowProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)m_ShadowWidth / (float)m_ShadowHeight, m_ShadowNearPlane, m_ShadowFarPlane);
    m_Shader_shadow = std::make_unique<Shader>("src/shaders/shadow.shader", Shader::VERTEX_GEOMETRY_FRAGMENT);
}

void TestView::UpdateShadowTransforms()
{
    m_ShadowTransforms.clear();
    glm::vec3 lightPos(m_LightSources[0].m_Position.x, m_LightSources[0].m_Position.y, m_LightSources[0].m_Position.z);
    m_ShadowTransforms.push_back(m_ShadowProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_ShadowTransforms.push_back(m_ShadowProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_ShadowTransforms.push_back(m_ShadowProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    m_ShadowTransforms.push_back(m_ShadowProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    m_ShadowTransforms.push_back(m_ShadowProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_ShadowTransforms.push_back(m_ShadowProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}

void TestView::RenderPointShadow()
{
    UpdateShadowTransforms();

    glViewport(0, 0, m_ShadowWidth, m_ShadowHeight);
    m_FrameBuffer->Bind();
    m_Renderer->Clear();
    m_Shader_shadow->Bind();

    for (int i = 0; i < 6; ++i)
    {
        m_Shader_shadow->SetUniformMat4f("u_ShadowMatrices[" + std::to_string(i) + "]", m_ShadowTransforms[i]);
    }
    m_Shader_shadow->SetUniform1f("u_FarPlane", m_ShadowFarPlane);
    m_Shader_shadow->SetUniform3f("u_LightPosition", m_LightSources[0].m_Position.x, m_LightSources[0].m_Position.y, m_LightSources[0].m_Position.z);
    
    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        BufferBox(m_Boxes[i]);        
        m_Shader_shadow->SetUniformMat4f("u_Model", m_BoxModelMats[i]);
        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader_shadow);
    }

    for (int i = 0; i < m_Spheres.size(); ++i)
    {
        BufferSphere(m_Spheres[i]);         
        m_Shader_shadow->SetUniformMat4f("u_Model", m_SphereModelMats[i]);
        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader_shadow);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TestView::RenderScene()
{
    const unsigned int textureSlot = 0;
    const unsigned int depthMapSlot = 1;

    glViewport(0, 0, m_Window->m_Width, m_Window->m_Height);
    m_Renderer->Clear();

    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_View", m_Camera->m_ViewMatrix);
    m_Shader->SetUniformMat4f("u_Proj", m_Camera->m_ProjectionMatrix);
    m_Shader->SetUniform1f("u_FarPlane", m_ShadowFarPlane);
    m_Shader->SetUniform1i("u_DepthMap", depthMapSlot);
    m_Shader->SetUniform1i("u_ApplyLighting", true);

    SetLightSources();
    
    m_TextureShadow->Bind(depthMapSlot);

    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        BufferBox(m_Boxes[i]);
        m_Shader->SetUniformMat4f("u_Model", m_BoxModelMats[i]);
        m_TextureBox->Bind(textureSlot);        
        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    }

    for (int i = 0; i < m_Spheres.size(); ++i)
    {
        BufferSphere(m_Spheres[i]);
        m_Shader->SetUniformMat4f("u_Model", m_SphereModelMats[i]);
        m_TextureEarth->Bind(textureSlot);  
        m_Renderer->Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
    }
    
    {
        glm::mat4 model(1.0f);
        m_Shader->SetUniform1i("u_ApplyLighting", false);
        m_Shader->SetUniformMat4f("u_Model", model);
        m_TextureRGB->Bind(textureSlot);
        m_Renderer->Draw(GL_LINES, *m_VAO_axes, *m_IBO_axes, *m_Shader);
    }
}

void TestView::DoTick()
{
    float locationMult = 0.005f;
        m_ObjectLocation += locationMult;
        if (m_ObjectLocation > (2.0f * M_PI))
            m_ObjectLocation = 0.0f;

    float rotationMult = 0.8f;
    m_ObjectRotation += rotationMult;
    if (m_ObjectRotation >= 720.0f)
        m_ObjectRotation =  0.0f;

    if (m_Spheres.size() >= 1)
    {
        m_Spheres[0].m_Center = Vec3(std::sin(m_ObjectLocation) * m_ObjectDistance, -300.0f, std::cos(m_ObjectLocation) * m_ObjectDistance);
        m_Spheres[0].m_Rotation.x = 0.0f;        
        m_Spheres[0].m_Rotation.y = m_ObjectRotation;
        m_Spheres[0].m_Rotation.z = 23.5;
        
    }

    if (m_Boxes.size() >= 1)
    {
        m_Boxes[0].m_Rotation.x = m_ObjectRotation;        
        m_Boxes[0].m_Rotation.y = m_ObjectRotation * 0.5f;
        m_Boxes[0].m_Rotation.z = 0.0f;
    }
}

void TestView::UpdateModelMats()
{
    m_BoxModelMats.clear();
    m_SphereModelMats.clear();

    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        glm::mat4 model(1.0f);
        glm::vec3 translation(m_Boxes[i].m_Center.x, m_Boxes[i].m_Center.y, m_Boxes[i].m_Center.z);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Boxes[i].m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_BoxModelMats.push_back(model);
    }

    for (int i = 0; i < m_Spheres.size(); ++i)
    {
        glm::mat4 model(1.0f);
        glm::vec3 translation(m_Spheres[i].m_Center.x, m_Spheres[i].m_Center.y, m_Spheres[i].m_Center.z);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Spheres[i].m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_SphereModelMats.push_back(model);
    }
}

void TestView::Render()
{
    m_Camera->Update();
    DoTick();
    UpdateModelMats();

    RenderPointShadow();
    RenderScene();
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
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}