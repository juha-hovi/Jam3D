#pragma once

#include "camera.h"

namespace Jam3D {

class OrthoCamera : public Camera
{
public:
    OrthoCamera(float left, float right, float bottom, float top, CameraBaseProperties properties);
    void Update() override;
    void UpdateAspectRatio(float aspectRatio);
    void UpdateProjMat(float left, float right, float bottom, float top);

    int m_Width;
    int m_Height;
    int m_LargestDim;
};

}