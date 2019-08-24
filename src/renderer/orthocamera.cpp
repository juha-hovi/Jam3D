#include "orthocamera.h"

namespace Jam3D {

OrthoCamera::OrthoCamera(float near, float far, Jam3D::Vec2<float> windowDim, std::shared_ptr<GLWindow> window)
    : Camera(near, far, windowDim, window)
{

}

}