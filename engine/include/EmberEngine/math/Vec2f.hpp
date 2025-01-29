#ifndef EE_Vec2f_HPP
#define EE_Vec2f_HPP

#include <string>

namespace EmberEngine
{
    class Vec2f
    {
        private:
            float x;
            float y;
        public:
            Vec2f();
            Vec2f(float x, float y);
            Vec2f(const Vec2f& other);
            Vec2f& operator=(const Vec2f& other);
        public: // -- Math -- //
            float length();
            float dot(const Vec2f& other);
            Vec2f normalize();
            Vec2f rotate(float angle);

            Vec2f operator+(const Vec2f& other);
            Vec2f operator-(const Vec2f& other);
            Vec2f operator*(const Vec2f& other);
            Vec2f operator/(const Vec2f& other);

            Vec2f operator+(const float other);
            Vec2f operator-(const float other);
            Vec2f operator*(const float other);
            Vec2f operator/(const float other);

            Vec2f operator+=(const Vec2f& other);
            Vec2f operator-=(const Vec2f& other);
            Vec2f operator*=(const Vec2f& other);
            Vec2f operator/=(const Vec2f& other);

            Vec2f operator+=(const float other);
            Vec2f operator-=(const float other);
            Vec2f operator*=(const float other);
            Vec2f operator/=(const float other);
        public: // -- Getters -- //
            std::string toString();
            float getX() const;
            float getY() const;
        public: // -- Setters -- //
            void setX(float x);
            void setY(float y);

    };
}

#endif