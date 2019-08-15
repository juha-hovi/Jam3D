#include "vec2.h"

namespace Jam3D {

Vec2::Vec2(float first, float second)
{
    x = first;
    y = second;
}

bool Vec2::operator==(const Vec2& rhs)
{
    return (this->x == rhs.x && this->y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs)
{
    return !(this->x == rhs.x && this->y == rhs.y);
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

}