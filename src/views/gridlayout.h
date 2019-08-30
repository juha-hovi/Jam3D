#pragma once

#include "viewport.h"

#include <vector>
#include <memory>

namespace Jam3D {

class GridLayout 
{
public:
    GridLayout(int rows, int cols, std::shared_ptr<Viewport> viewport);

    void InitViewports();
    void ResizeViewports(int row, int col, int pos, bool vertical, bool horizontal);

    int m_Rows;
    int m_Cols;

    std::shared_ptr<Viewport> m_BaseViewport;

    std::vector<Viewport> m_Viewports;
};

}