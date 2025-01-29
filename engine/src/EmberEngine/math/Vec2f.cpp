#include <EmberEngine/math/Vec2f.hpp>
#include <EmberEngine/math/Math.hpp>

#include <cmath>

namespace EmberEngine
{
    Vec2f::Vec2f()
    {
        this->x = 0.0f;
        this->y = 0.0f;
    }

    Vec2f::Vec2f(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Vec2f::Vec2f(const Vec2f& other)
    {
        this->x = other.x;
        this->y = other.y;
    }

    Vec2f& Vec2f::operator=(const Vec2f& other)
    {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }

    // -- Math -- //
    float Vec2f::length()
    {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    float Vec2f::dot(const Vec2f& other)
    {
        return this->x * other.x + this->y * other.y;
    }

    Vec2f Vec2f::normalize()
    {
        float len = this->length();
        return Vec2f(this->x / len, this->y / len);
    }

    Vec2f Vec2f::rotate(float angle)
    {
        float rad = Math::toRadians(angle);
        float cos = std::cos(rad);
        float sin = std::sin(rad);

        return Vec2f(this->x * cos - this->y * sin, this->x * sin + this->y * cos);
    }

    Vec2f Vec2f::operator+(const Vec2f& other)
    {
        return Vec2f(this->x + other.x, this->y + other.y);
    }

    Vec2f Vec2f::operator-(const Vec2f& other)
    {
        return Vec2f(this->x - other.x, this->y - other.y);
    }

    Vec2f Vec2f::operator*(const Vec2f& other)
    {
        return Vec2f(this->x * other.x, this->y * other.y);
    }

    Vec2f Vec2f::operator/(const Vec2f& other)
    {
        return Vec2f(this->x / other.x, this->y / other.y);
    }

    Vec2f Vec2f::operator+(const float other)
    {
        return Vec2f(this->x + other, this->y + other);
    }

    Vec2f Vec2f::operator-(const float other)
    {
        return Vec2f(this->x - other, this->y - other);
    }

    Vec2f Vec2f::operator*(const float other)
    {
        return Vec2f(this->x * other, this->y * other);
    }

    Vec2f Vec2f::operator/(const float other)
    {
        return Vec2f(this->x / other, this->y / other);
    }

    Vec2f Vec2f::operator+=(const Vec2f& other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    Vec2f Vec2f::operator-=(const Vec2f& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    Vec2f Vec2f::operator*=(const Vec2f& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }

    Vec2f Vec2f::operator/=(const Vec2f& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }

    Vec2f Vec2f::operator+=(const float other)
    {
        this->x += other;
        this->y += other;
        return *this;
    }

    Vec2f Vec2f::operator-=(const float other)
    {
        this->x -= other;
        this->y -= other;
        return *this;
    }

    Vec2f Vec2f::operator*=(const float other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }

    Vec2f Vec2f::operator/=(const float other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }

    // -- Getters -- //
    std::string Vec2f::toString()
    {
        return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")";
    }

    float Vec2f::getX() const
    {
        return this->x;
    }

    float Vec2f::getY() const
    {
        return this->y;
    }

    // -- Setters -- //
    void Vec2f::setX(float x)
    {
        this->x = x;
    }

    void Vec2f::setY(float y)
    {
        this->y = y;
    }


}