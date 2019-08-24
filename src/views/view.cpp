#include "view.h"
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

}