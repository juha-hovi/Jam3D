#pragma once

namespace Jam3D {

template <typename T>
class Vec4 
{
public:
    // Constructors
    Vec4()
        : x(0), y(0), z(0), w(0)
        { };
    Vec4(T first, T second, T third, T fourth) 
        : x(first), y(second), z(third), w(fourth) 
        { };
    Vec4(const Vec4<T>& orig) 
        { x = orig.x; y = orig.y; z = orig.z; w = orig.w; };

    // Operator overloading
    bool operator!=(const Vec4<T>& rhs)
        { return !(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w); };
    bool operator==(const Vec4<T>& rhs)
        { return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w); };
    Vec4<T>& operator+=(const Vec4<T>& rhs)
        { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; this->w += rhs.w; return *this; };
    Vec4<T> operator+(const Vec4<T>& rhs)
        { return Vec4<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w); };
    Vec4<T> operator-(const Vec4<T>& rhs)
        { return Vec4<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w); };
    Vec4<T> operator/(const T& rhs)
        { return Vec4<T>(this->x / rhs, this->y / rhs, this->z / rhs, this->w / rhs.w); };

    // Variables
    union
    {
        struct
        {
            T x, y, z, w;
        };
        struct
        {
            T r, g, b, a;
        };
        struct 
        {
            T x0, y0, x1, y1;
        };
                
    };
};

}