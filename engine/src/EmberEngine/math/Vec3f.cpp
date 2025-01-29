#include <EmberEngine/math/Vec3f.hpp>

#include <cmath>

namespace EmberEngine
{
    Vec3f::Vec3f()
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    Vec3f::Vec3f(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3f::Vec3f(const Vec3f& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    Vec3f& Vec3f::operator=(const Vec3f& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    // -- Math -- //
    float Vec3f::length()
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    float Vec3f::dot(const Vec3f& other)
    {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    float Vec3f::cross(const Vec3f& other)
    {
        return Vec3f(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x).length();
    }

    Vec3f Vec3f::normalize()
    {
        float len = this->length();
        return Vec3f(this->x / len, this->y / len, this->z / len);
    }

    Vec3f Vec3f::operator+(const Vec3f& other)
    {
        return Vec3f(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    Vec3f Vec3f::operator-(const Vec3f& other)
    {
        return Vec3f(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    Vec3f Vec3f::operator*(const Vec3f& other)
    {
        return Vec3f(this->x * other.x, this->y * other.y, this->z * other.z);
    }

    Vec3f Vec3f::operator/(const Vec3f& other)
    {
        return Vec3f(this->x / other.x, this->y / other.y, this->z / other.z);
    }   

    Vec3f Vec3f::operator+(const float other)
    {
        return Vec3f(this->x + other, this->y + other, this->z + other);
    }

    Vec3f Vec3f::operator-(const float other)
    {
        return Vec3f(this->x - other, this->y - other, this->z - other);
    }

    Vec3f Vec3f::operator*(const float other)
    {
        return Vec3f(this->x * other, this->y * other, this->z * other);
    }

    Vec3f Vec3f::operator/(const float other)
    {
        return Vec3f(this->x / other, this->y / other, this->z / other);
    }

    Vec3f Vec3f::operator+=(const Vec3f& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    Vec3f Vec3f::operator-=(const Vec3f& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    Vec3f Vec3f::operator*=(const Vec3f& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }

    Vec3f Vec3f::operator/=(const Vec3f& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }

    Vec3f Vec3f::operator+=(const float other)
    {
        this->x += other;
        this->y += other;
        this->z += other;
        return *this;
    }

    Vec3f Vec3f::operator-=(const float other)
    {
        this->x -= other;
        this->y -= other;
        this->z -= other;
        return *this;
    }

    Vec3f Vec3f::operator*=(const float other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }

    Vec3f Vec3f::operator/=(const float other)
    {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        return *this;
    }

    // -- Getters -- //
    std::string Vec3f::toString()
    {
        return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ")";
    }

    float Vec3f::getX() const
    {
        return this->x;
    }

    float Vec3f::getY() const
    {
        return this->y;
    }

    float Vec3f::getZ() const
    {
        return this->z;
    }

    // -- Setters -- //
    void Vec3f::setX(float x)
    {
        this->x = x;
    }

    void Vec3f::setY(float y)
    {
        this->y = y;
    }

    void Vec3f::setZ(float z)
    {
        this->z = z;
    }


}