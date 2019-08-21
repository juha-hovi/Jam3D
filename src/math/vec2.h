#pragma once

namespace Jam3D {

template <typename T>
class Vec2 
{
public:
    // Constructors
    Vec2()
        : x(0), y(0)
        { };
    Vec2(T first, T second) 
        : x(first), y(second)
        { };
    Vec2(const Vec2<T>& orig) 
        { x = orig.x; y = orig.y; };

    // Operator overloading
    bool operator!=(const Vec2<T>& rhs)
        { return !(this->x == rhs.x && this->y == rhs.y); };
    bool operator==(const Vec2<T>& rhs)
        { return (this->x == rhs.x && this->y == rhs.y); };
    Vec2<T>& operator+=(const Vec2<T>& rhs)
        { this->x += rhs.x; this->y += rhs.y; return *this; };
    Vec2<T> operator+(const Vec2<T>& rhs)
        { return Vec2<T>(this->x + rhs.x, this->y + rhs.y); };
    Vec2<T> operator-(const Vec2<T>& rhs)
        { return Vec2<T>(this->x - rhs.x, this->y - rhs.y); };
    Vec2<T> operator/(const T& rhs)
        { return Vec2<T>(this->x / rhs, this->y / rhs); };

    // Variables
    T x, y;
};

}