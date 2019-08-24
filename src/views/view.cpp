#include "view.h"
namespace Jam3D {

View * View::m_CurrentView;

View::View(std::shared_ptr<GLWindow> window)
    : m_Window(window)
{
    m_Renderer = std::make_unique<Renderer>();
}

}