#include "vec3.h"

namespace Jam3D {

Vec3::Vec3()
{
    
}

Vec3::Vec3(float first, float second, float third)
{
    x = first;
    y = second;
    z = third;
}

Vec3::Vec3(const Vec3& orig)
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
}

bool Vec3::operator==(const Vec3& rhs)
{
    return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
}

bool Vec3::operator!=(const Vec3& rhs)
{
    return !(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
}

Vec3& Vec3::operator+=(const Vec3& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

Vec3 Vec3::operator+(const Vec3& rhs)
{
    return Vec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

Vec3 Vec3::operator-(const Vec3& rhs)
{
    return Vec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

Vec3 Vec3::operator/(const float& rhs)
{
    return Vec3(this->x / rhs, this->y / rhs, this->z / rhs);
}

}