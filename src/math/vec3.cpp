#include "vec3.h"

namespace Jam3D {

Vec3::Vec3(float first, float second, float third)
{
    x = first;
    y = second;
    z = third;
}

bool Vec3::operator==(const Vec3& rhs)
{
    return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
}

bool Vec3::operator!=(const Vec3& rhs)
{
    return !(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
}

}