#pragma once

#include <vector>

#include "viewport.h"
#include "glwindow.h"
#include "renderer.h"

namespace Jam3D {

class View
{
public:
    View(std::shared_ptr<GLWindow> window);

    virtual void Render() = 0;

    std::shared_ptr<GLWindow> m_Window;

    std::unique_ptr<Renderer> m_Renderer;

    std::vector<Viewport> m_Viewports;
};

}