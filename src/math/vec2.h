#pragma once

namespace Jam3D {

class Vec2 
{
public:
    Vec2(float first, float second);

    bool operator==(const Vec2& rhs);
    bool operator!=(const Vec2& rhs);

    union
    {
        struct
        {
            float x, y;
        };
    };
};

}