#ifndef EE_Vec3f_HPP
#define EE_Vec3f_HPP

#include <string>

namespace EmberEngine
{
    class Vec3f
    {
        private:
            float x;
            float y;
            float z;
        public:
            Vec3f();
            Vec3f(float x, float y, float z);
            Vec3f(const Vec3f& other);
            Vec3f& operator=(const Vec3f& other);
        public: // -- Math -- //
            float length();
            float dot(const Vec3f& other);
            float cross(const Vec3f& other);
            Vec3f normalize();

            Vec3f operator+(const Vec3f& other);
            Vec3f operator-(const Vec3f& other);
            Vec3f operator*(const Vec3f& other);
            Vec3f operator/(const Vec3f& other);

            Vec3f operator+(const float other);
            Vec3f operator-(const float other);
            Vec3f operator*(const float other);
            Vec3f operator/(const float other);

            Vec3f operator+=(const Vec3f& other);
            Vec3f operator-=(const Vec3f& other);
            Vec3f operator*=(const Vec3f& other);
            Vec3f operator/=(const Vec3f& other);

            Vec3f operator+=(const float other);
            Vec3f operator-=(const float other);
            Vec3f operator*=(const float other);
            Vec3f operator/=(const float other);
        public: // -- Getters -- //
            std::string toString();
            float getX() const;
            float getY() const;
            float getZ() const;
        public: // -- Setters -- //
            void setX(float x);
            void setY(float y);
            void setZ(float z);
    };
}

#endif