#pragma once

namespace Jam3D {

template <class T>
class Vec3 
{
public:
    // Constructors
    Vec3()
        : x(0), y(0), z(0)
        { };
    Vec3(T first, T second, T third) 
        : x(first), y(second), z(third) 
        { };
    Vec3(const Vec3<T>& orig) 
        { x = orig.x; y = orig.y; z = orig.z; };

    // Operator overloading
    bool operator!=(const Vec3<T>& rhs)
        { return !(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z); };
    bool operator==(const Vec3<T>& rhs)
        { return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z); };
    Vec3<T>& operator+=(const Vec3<T>& rhs)
        { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; return *this; };
    Vec3<T> operator+(const Vec3<T>& rhs)
        { return Vec3<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z); };
    Vec3<T> operator-(const Vec3<T>& rhs)
        { return Vec3<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z); };
    Vec3<T> operator/(const T& rhs)
        { return Vec3<T>(this->x / rhs, this->y / rhs, this->z / rhs); };

    // Variables
    union
    {
        struct
        {
            T x, y, z;
        };
        struct
        {
            T r, g, b;
        };        
    };
};

}