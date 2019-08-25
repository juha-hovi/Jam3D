#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "vec4.h"

namespace Jam3D {

class MathHelper {
public:
    template <typename T>
    static glm::vec4 Vec4ToGlm(Jam3D::Vec4<T> in)
    {
        return glm::vec4(in.x, in.y, in.z, in.w);
    };

};

}