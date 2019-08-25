#pragma once

#include "shape.h"

namespace Jam3D {

class Plane : public Shape
{
public:
    enum : unsigned int {
        XZ, XY, YZ
    };

    Plane(unsigned int type);
    void Update() override;
};

}