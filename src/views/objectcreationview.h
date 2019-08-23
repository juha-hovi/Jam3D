#pragma once

#include "view.h"

namespace Jam3D {

class ObjectCreationView : public View
{
public:
    ObjectCreationView(std::shared_ptr<GLWindow> window);
    void Render() override;

    void InitViewports();

    int m_UpperLeftViewportIndex;
    int m_UpperRightViewportIndex;
    int m_LowerLeftViewportIndex;
    int m_LowerRightViewportIndex;
};

}