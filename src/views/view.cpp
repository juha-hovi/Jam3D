#include "view.h"
namespace Jam3D {

// Global for input handling
View * View::m_CurrentView;

View::View(std::shared_ptr<GLWindow> window)
    : m_Window(window)
{
    m_Renderer = std::make_unique<Renderer>();

    m_Boxes = std::make_shared<std::vector<Box>>();
    m_BoxModelMats = std::make_shared<std::vector<glm::mat4>>();
    m_Spheres = std::make_shared<std::vector<Sphere>>();
    m_SphereModelMats = std::make_shared<std::vector<glm::mat4>>();
    m_LightSources = std::make_shared<std::vector<LightSource>>();
}

}