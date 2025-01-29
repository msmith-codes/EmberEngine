#include <EmberEngine/math/Math.hpp>

namespace EmberEngine
{
    float Math::PI = 3.14159265359f;

    float Math::toRadians(float degrees)
    {
        return degrees * (PI / 180.0f);
    }

    float Math::toDegrees(float radians)
    {
        return radians * (180.0f / PI);
    }
}