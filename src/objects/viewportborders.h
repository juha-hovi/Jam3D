#pragma once

#include <vector>
#include <memory>

#include "shape.h"
#include "viewport.h"
#include "glwindow.h"

namespace Jam3D {

class ViewportBorders : public Shape
{
public:
    ViewportBorders(std::vector<Viewport>& viewports, std::shared_ptr<GLWindow>& window);
    void Update() override;

    std::vector<float> m_Positions;
    std::vector<unsigned int> m_Indices;
};

}