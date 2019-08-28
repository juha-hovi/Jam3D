#include "viewportborders.h"

namespace Jam3D {

ViewportBorders::ViewportBorders(std::vector<Viewport>& viewports, std::shared_ptr<GLWindow>& window)
{
     for (int i = 0; i < viewports.size() - 1; ++i)
    {
        float xBottomLeft = 2.0f * ((float)viewports[i].m_Corners.x0 - 1) / (float)window->m_Width - 1.0f;
        float yBottomLeft = 2.0f * ((float)viewports[i].m_Corners.y0 - 1) / (float)window->m_Height - 1.0f;
        float xBottomRight = 2.0f * ((float)viewports[i].m_Corners.x0 + (float)viewports[i].m_Corners.x1 + 1) / (float)window->m_Width - 1.0f;
        float yBottomRight = 2.0f * ((float)viewports[i].m_Corners.y0 - 1) / (float)window->m_Height - 1.0f;
        float xTopLeft = 2.0f * ((float)viewports[i].m_Corners.x0 - 1) / (float)window->m_Width - 1.0f;
        float yTopLeft = 2.0f * ((float)viewports[i].m_Corners.y0 + (float)viewports[i].m_Corners.y1 + 1) / (float)window->m_Height - 1.0f;
        float xTopRight = 2.0f * ((float)viewports[i].m_Corners.x0 + (float)viewports[i].m_Corners.x1 + 1) / (float)window->m_Width - 1.0f;
        float yTopRight = 2.0f * ((float)viewports[i].m_Corners.y0 + (float)viewports[i].m_Corners.y1 + 1) / (float)window->m_Height - 1.0f;

        float z = -1.0f;

        // Bottom
        m_Positions.push_back(xBottomLeft);
        m_Positions.push_back(yBottomLeft);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }
        m_Positions.push_back(xBottomRight);
        m_Positions.push_back(yBottomRight);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }
        // Top
        m_Positions.push_back(xTopLeft);
        m_Positions.push_back(yTopLeft);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }
        m_Positions.push_back(xTopRight);
        m_Positions.push_back(yTopRight);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }

        // Left
        m_Positions.push_back(xBottomLeft);
        m_Positions.push_back(yBottomLeft);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }
        m_Positions.push_back(xTopLeft);
        m_Positions.push_back(yTopLeft);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }

        // Right
        m_Positions.push_back(xBottomRight);
        m_Positions.push_back(yBottomRight);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }
        m_Positions.push_back(xTopRight);
        m_Positions.push_back(yTopRight);
        m_Positions.push_back(z);
        for (int j = 0; j < 5; ++j)
        {
            m_Positions.push_back(0.0f);
        }
    }
    for (unsigned int i = 0; i < m_Positions.size(); ++i)
    {
        m_Indices.push_back(i);
    }
}

void ViewportBorders::Update()
{

}

}