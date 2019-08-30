#include "gridlayout.h"
#include "vec4.h"

namespace Jam3D {

GridLayout::GridLayout(int rows, int cols, std::shared_ptr<Viewport> viewport)
    : m_Rows(rows), m_Cols(cols), m_BaseViewport(viewport)
{
    InitViewports();
}

// Viewport index: col + row * m_Cols
void GridLayout::InitViewports()
{
    int x1 = m_BaseViewport->m_Corners.x1 / m_Cols;
    int y1 = m_BaseViewport->m_Corners.y1 / m_Rows;
    for (int row = 0; row < m_Rows; ++row)
    {
        int y0 = m_BaseViewport->m_Corners.y0 + (row * y1);
        for (int col = 0; col < m_Cols; ++col)
        {
            int x0 = m_BaseViewport->m_Corners.x0 + (col * x1);
            m_Viewports.push_back(Viewport(Jam3D::Vec4<int>(x0, y0, x1, y1)));
        }
    }
}

// In: row/col = position of cell under/left of the resized border
// delta = change in position
// vertical/horizontal = border drag affects row/col
void GridLayout::ResizeViewports(int row, int col, int delta, bool vertical, bool horizontal)
{
    if (vertical)
    {
        int lowerViewportIndex = col + row * m_Cols;
        int upperViewportIndex = lowerViewportIndex + m_Cols;
    }
    else if (horizontal)
    {
        int leftViewportIndex = col + row * m_Cols;
        int rightViewportIndex = leftViewportIndex + 1;
    }
}
    
}