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

TestView::TestView(std::shared_ptr<GLWindow> window) : View(window),
    m_BoxCenter(0.0f, 0.0f, 0.0f), m_BoxDimensions(0.0f, 0.0f, 0.0f), m_BoxRotation(0.0f, 0.0f, 0.0f), 
    m_SphereCenter(0.0f, 0.0f, 0.0f), m_SphereRadius(100.0f), m_SphereSectorCount(10), m_SphereStackCount(10),
    m_ObjectRotation(0.0f), m_ObjectLocation(0.0f), m_ObjectDistance(500.0f),
    m_LightType(LightSource::POINT_LIGHT), m_LightPosition(Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f)), m_LightColor(Jam3D::Vec3<float>(1.0f, 1.0f, 1.0f)),
    m_LightIntensity(1.0f), m_ShadowNearPlane(1.0f), m_ShadowFarPlane(5000), m_ShadowProjectionMatrix(glm::mat4(1.0f)),
    m_NormalViewportIndex(-1), m_ShadowViewportIndex(-1)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  

    AddBox(Jam3D::Vec3<float>(0.0f, -200.0f, 0.0f), Jam3D::Vec3<float>(70.0f, 50.0f, 90.0f), Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f));
    AddBox(Jam3D::Vec3<float>(0.0f, -500.0f, 0.0f), Jam3D::Vec3<float>(2000.0f, 10.0f, 2000.0f), Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f));
    AddSphere(100.0f, Jam3D::Vec3<float>(std::sin(m_ObjectLocation) * m_ObjectDistance, -300.0f, std::cos(m_ObjectLocation) * m_ObjectDistance), 20, 20);
    AddLightSource(LightSource::POINT_LIGHT, Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f), Jam3D::Vec3<float>(1.0f, 1.0f, 1.0f), 1.0f);

    InitRendering();
    InitAxes();
    InitPointShadow();
}

void TestView::InitRendering()
{
    m_LayoutShape = std::make_unique<VertexBufferLayout>();
    m_LayoutShape->Push<float>(3);
    m_LayoutShape->Push<float>(2);
    m_LayoutShape->Push<float>(3);

    m_ShaderNormal = std::make_unique<Shader>("src/shaders/basic3d.shader", Shader::VERTEX_FRAGMENT);

    m_TextureBox = std::make_unique<Texture2D>("res/tex_test_full.png");
    m_TextureEarth = std::make_unique<Texture2D>("res/earth2048.bmp");
    m_TextureRGB = std::make_unique<Texture2D>("res/rgb.png");

    float fov = 45.0f;
    float near = 1.0f;
    float far = 5000.0f;
    Vec2<float> windowDim({(float)m_Window->m_Width, (float)m_Window->m_Height});
    m_Camera = std::make_shared<PerspectiveCamera>(fov, near, far, windowDim, m_Window);

    m_NormalViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Viewport(Jam3D::Vec4<int>(0, 0, m_Window->m_Width, m_Window->m_Height)));
}

void TestView::InitAxes()
{
    m_Axes = std::make_unique<Axes>();
    m_VAOAxes = std::make_unique<VertexArray>();
    m_VBOAxes = std::make_unique<VertexBuffer>(m_Axes->m_VertexData.data(), m_Axes->m_VertexData.size() * sizeof(float));
    m_LayoutAxes = std::make_unique<VertexBufferLayout>();
    m_LayoutAxes->Push<float>(3);
    m_LayoutAxes->Push<float>(2);
    m_LayoutAxes->Push<float>(3);
    m_VAOAxes->AddBuffer(*m_VBOAxes, *m_LayoutAxes);
    m_IBOAxes = std::make_unique<IndexBuffer>(m_Axes->m_Indices.data(), m_Axes->m_Indices.size());
}

void TestView::AddBox(Jam3D::Vec3<float> center, Jam3D::Vec3<float> dimensions, Jam3D::Vec3<float> rotation)
{
    m_Boxes.push_back(Box(center, dimensions, rotation));
}

void TestView::AddSphere(float radius, Jam3D::Vec3<float> center, int sectorCount, int stackCount)
{
    m_Spheres.push_back(Sphere(radius, center, sectorCount, stackCount));
}

void TestView::AddLightSource(unsigned int type, Jam3D::Vec3<float> position_or_direction, Jam3D::Vec3<float> color, float intensity)
{
    if (m_LightSources.size() <= 10)
    {
        m_LightSources.push_back(LightSource(type, position_or_direction, color, intensity));
    }
    else
    {
        Jam3D::Log::Warning("Already at max light sources! Current max: " + m_LightSources.size());
    }    
}

void TestView::BufferShape(const Shape& shape)
{
    m_VAOShape = std::make_unique<VertexArray>();
    m_VBOShape = std::make_unique<VertexBuffer>(shape.m_VertexData.data(), shape.m_VertexData.size() * sizeof(float));    
    m_VAOShape->AddBuffer(*m_VBOShape, *m_LayoutShape);
    m_IBOShape = std::make_unique<IndexBuffer>(shape.m_Indices.data(), shape.m_Indices.size());
}

void TestView::SetLightSources()
{
    for (int i = 0; i < m_LightSources.size(); ++i)
    {
        std::string prefix = "u_LightSources[" + std::to_string(i) + "].";
        
        m_ShaderNormal->SetUniform3f((prefix + "lightPosition").c_str(), m_LightSources[i].m_Position.x, m_LightSources[i].m_Position.y, m_LightSources[i].m_Position.z);
        m_ShaderNormal->SetUniform3f((prefix + "lightColor").c_str(), m_LightSources[i].m_Color.r, m_LightSources[i].m_Color.g, m_LightSources[i].m_Color.b);
        m_ShaderNormal->SetUniform1f((prefix + "lightIntensity").c_str(), m_LightSources[i].m_Intensity);
    }

    m_ShaderNormal->SetUniform1i("u_LightSourceCount", m_LightSources.size());
}

void TestView::InitPointShadow()
{
    m_FrameBuffer = std::make_unique<FrameBuffer>();
    m_TextureShadow = std::make_unique<TextureCubeMap>(m_ShadowWidth, m_ShadowHeight);
    m_FrameBuffer->AttachTexture(m_TextureShadow->GetRendererID());

    m_ShadowProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)m_ShadowWidth / (float)m_ShadowHeight, m_ShadowNearPlane, m_ShadowFarPlane);
    m_ShaderShadow = std::make_unique<Shader>("src/shaders/shadow.shader", Shader::VERTEX_GEOMETRY_FRAGMENT);

    m_ShadowViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Viewport(Jam3D::Vec4<int>(0, 0, m_ShadowWidth, m_ShadowHeight)));
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

    m_Viewports[m_ShadowViewportIndex].Use();
    m_FrameBuffer->Bind();
    m_Renderer->Clear();
    m_ShaderShadow->Bind();

    for (int i = 0; i < 6; ++i)
    {
        m_ShaderShadow->SetUniformMat4f("u_ShadowMatrices[" + std::to_string(i) + "]", m_ShadowTransforms[i]);
    }
    m_ShaderShadow->SetUniform1f("u_FarPlane", m_ShadowFarPlane);
    m_ShaderShadow->SetUniform3f("u_LightPosition", m_LightSources[0].m_Position.x, m_LightSources[0].m_Position.y, m_LightSources[0].m_Position.z);
    
    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        BufferShape(m_Boxes[i]);        
        m_ShaderShadow->SetUniformMat4f("u_Model", m_BoxModelMats[i]);
        m_Renderer->Draw(GL_TRIANGLES, *m_VAOShape, *m_IBOShape, *m_ShaderShadow);
    }

    for (int i = 0; i < m_Spheres.size(); ++i)
    {
        BufferShape(m_Spheres[i]);         
        m_ShaderShadow->SetUniformMat4f("u_Model", m_SphereModelMats[i]);
        m_Renderer->Draw(GL_TRIANGLES, *m_VAOShape, *m_IBOShape, *m_ShaderShadow);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TestView::RenderScene()
{
    const unsigned int textureSlot = 0;
    const unsigned int depthMapSlot = 1;

    m_Viewports[m_NormalViewportIndex].Use();
    m_Renderer->Clear();

    m_ShaderNormal->Bind();
    m_ShaderNormal->SetUniformMat4f("u_View", m_Camera->m_ViewMatrix);
    m_ShaderNormal->SetUniformMat4f("u_Proj", m_Camera->m_ProjectionMatrix);
    m_ShaderNormal->SetUniform1f("u_FarPlane", m_ShadowFarPlane);
    m_ShaderNormal->SetUniform1i("u_DepthMap", depthMapSlot);
    m_ShaderNormal->SetUniform1i("u_ApplyLighting", true);

    SetLightSources();
    
    m_TextureShadow->Bind(depthMapSlot);

    for (int i = 0; i < m_Boxes.size(); ++i)
    {
        BufferShape(m_Boxes[i]);
        m_ShaderNormal->SetUniformMat4f("u_Model", m_BoxModelMats[i]);
        m_TextureBox->Bind(textureSlot);        
        m_Renderer->Draw(GL_TRIANGLES, *m_VAOShape, *m_IBOShape, *m_ShaderNormal);
    }

    for (int i = 0; i < m_Spheres.size(); ++i)
    {
        BufferShape(m_Spheres[i]);
        m_ShaderNormal->SetUniformMat4f("u_Model", m_SphereModelMats[i]);
        m_TextureEarth->Bind(textureSlot);  
        m_Renderer->Draw(GL_TRIANGLES, *m_VAOShape, *m_IBOShape, *m_ShaderNormal);
    }
    
    {
        glm::mat4 model(1.0f);
        m_ShaderNormal->SetUniform1i("u_ApplyLighting", false);
        m_ShaderNormal->SetUniformMat4f("u_Model", model);
        m_TextureRGB->Bind(textureSlot);
        m_Renderer->Draw(GL_LINES, *m_VAOAxes, *m_IBOAxes, *m_ShaderNormal);
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
        m_Spheres[0].m_Center = Jam3D::Vec3<float>(std::sin(m_ObjectLocation) * m_ObjectDistance, -300.0f, std::cos(m_ObjectLocation) * m_ObjectDistance);
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