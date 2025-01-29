#ifndef EE_Math_HPP
#define EE_Math_HPP

namespace EmberEngine
{
    class Math
    {
        public:
            static float PI;
        public:
            static float toRadians(float degrees);
            static float toDegrees(float radians);
    };
}

#endif