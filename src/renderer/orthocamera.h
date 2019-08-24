#pragma once

#include "camera.h"

namespace Jam3D {

class OrthoCamera : Camera
{
public:
    OrthoCamera(float near, float far, Jam3D::Vec2<float> windowDim, std::shared_ptr<GLWindow> window);

};

}