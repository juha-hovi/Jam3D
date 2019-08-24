#include "view.h"
#include "log.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Jam3D {

// Global for shared input callbacks
View * View::m_CurrentView;

// Global for shared object storage
std::vector<Box> View::m_Boxes;
std::vector<glm::mat4> View::m_BoxModelMats;
std::vector<Sphere> View::m_Spheres;
std::vector<glm::mat4> View::m_SphereModelMats;
std::vector<LightSource> View::m_LightSources;

View::View(std::shared_ptr<GLWindow> window)
    : m_Window(window), m_ShadowNearPlane(1.0f), m_ShadowFarPlane(5000), m_ShadowProjectionMatrix(glm::mat4(1.0f))
{
    m_Renderer = std::make_unique<Renderer>();

    InitRendering();
    InitAxes();
    InitPointShadow();
}

void View::AddBox(Jam3D::Vec3<float> center, Jam3D::Vec3<float> dimensions, Jam3D::Vec3<float> rotation)
{
    m_Boxes.push_back(Box(center, dimensions, rotation));
}

void View::AddSphere(float radius, Jam3D::Vec3<float> center, int sectorCount, int stackCount)
{
    m_Spheres.push_back(Sphere(radius, center, sectorCount, stackCount));
}

void View::AddLightSource(unsigned int type, Jam3D::Vec3<float> position_or_direction, Jam3D::Vec3<float> color, float intensity)
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

void View::InitRendering()
{
    m_LayoutShape = std::make_unique<VertexBufferLayout>();
    m_LayoutShape->Push<float>(3);
    m_LayoutShape->Push<float>(2);
    m_LayoutShape->Push<float>(3);

    m_ShaderNormal = std::make_unique<Shader>("src/shaders/basic3d.shader", Shader::VERTEX_FRAGMENT);

    m_TextureBox = std::make_unique<Texture2D>("res/tex_test_full.png");
    m_TextureEarth = std::make_unique<Texture2D>("res/earth2048.bmp");
    m_TextureRGB = std::make_unique<Texture2D>("res/rgb.png");
}

void View::InitAxes()
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

void View::InitPointShadow()
{
    m_FrameBuffer = std::make_unique<FrameBuffer>();
    m_TextureShadow = std::make_unique<TextureCubeMap>(m_ShadowWidth, m_ShadowHeight);
    m_FrameBuffer->AttachTexture(m_TextureShadow->GetRendererID());

    m_ShadowProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)m_ShadowWidth / (float)m_ShadowHeight, m_ShadowNearPlane, m_ShadowFarPlane);
    m_ShaderShadow = std::make_unique<Shader>("src/shaders/shadow.shader", Shader::VERTEX_GEOMETRY_FRAGMENT);
}

void View::UpdateModelMats()
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

void View::BufferShape(const Shape& shape)
{
    m_VAOShape = std::make_unique<VertexArray>();
    m_VBOShape = std::make_unique<VertexBuffer>(shape.m_VertexData.data(), shape.m_VertexData.size() * sizeof(float));    
    m_VAOShape->AddBuffer(*m_VBOShape, *m_LayoutShape);
    m_IBOShape = std::make_unique<IndexBuffer>(shape.m_Indices.data(), shape.m_Indices.size());
}

void View::UpdateShadowTransforms()
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

void View::RenderPointShadow()
{
    UpdateShadowTransforms();

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

void View::RenderScene(Camera &camera)
{
    const unsigned int textureSlot = 0;
    const unsigned int depthMapSlot = 1;

    m_ShaderNormal->Bind();
    m_ShaderNormal->SetUniformMat4f("u_View", camera.m_ViewMatrix);
    m_ShaderNormal->SetUniformMat4f("u_Proj", camera.m_ProjectionMatrix);
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

void View::SetLightSources()
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

}