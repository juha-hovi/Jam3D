#pragma once

namespace Jam3D {

class Vec3 
{
public:
    Vec3();
    Vec3(float first, float second, float third);
    Vec3(const Vec3& orig);

    bool operator==(const Vec3& rhs);
    bool operator!=(const Vec3& rhs);
    Vec3& operator+=(const Vec3& rhs);
    Vec3 operator+(const Vec3& rhs);
    Vec3 operator-(const Vec3& rhs);
    Vec3 operator/(const float& rhs);

    union
    {
        struct
        {
            float x, y, z;
        };
        struct
        {
            float r, g, b;
        };        
    };
};

}