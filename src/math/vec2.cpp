#include "vec2.h"

namespace Jam3D {

Vec2::Vec2(float first, float second, float third)
{
    x = first;
    y = second;
    z = third;
}

bool Vec2::operator==(const Vec3& rhs)
{
    return (this->x == rhs.x && this->y == rhs.y);
}

bool Vec2::operator!=(const Vec3& rhs)
{
    return !(this->x == rhs.x && this->y == rhs.y);
}

}