#pragma once

#include "shape.h"

namespace Jam3D {

class Axes : public Shape
{
public:
    Axes();
    void Update() override;
};

}