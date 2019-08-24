#include "view.h"
#include "log.h"

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
    : m_Window(window)
{
    m_Renderer = std::make_unique<Renderer>();
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

void View::BufferShape(const Shape& shape)
{
    m_VAOShape = std::make_unique<VertexArray>();
    m_VBOShape = std::make_unique<VertexBuffer>(shape.m_VertexData.data(), shape.m_VertexData.size() * sizeof(float));    
    m_VAOShape->AddBuffer(*m_VBOShape, *m_LayoutShape);
    m_IBOShape = std::make_unique<IndexBuffer>(shape.m_Indices.data(), shape.m_Indices.size());
}

}