#include <EmberEngine/math/Quatf.hpp>

#include <EmberEngine/math/Vec3f.hpp>

#include <cmath>

namespace EmberEngine
{
    Quatf::Quatf()
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 1.0f;
    }

    Quatf::Quatf(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Quatf::Quatf(const Quatf& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
    }

    Quatf& Quatf::operator=(const Quatf& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
        return *this;
    }

    // -- Math -- //
    float Quatf::length()
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
    }

    Quatf Quatf::normalize()
    {
        float len = this->length();
        return Quatf(this->x / len, this->y / len, this->z / len, this->w / len);
    }

    Quatf Quatf::conjugate()
    {
        return Quatf(-this->x, -this->y, -this->z, this->w);
    }

    Quatf Quatf::operator*(const Quatf& other)
    {
        float w_ = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
        float x_ = this->x * other.w + this->w * other.x + this->y * other.z - this->z * other.y;
        float y_ = this->y * other.w + this->w * other.y + this->z * other.x - this->x * other.z;
        float z_ = this->z * other.w + this->w * other.z + this->x * other.y - this->y * other.x;
        return Quatf(x_, y_, z_, w_);
    }

    Quatf Quatf::operator*=(const Quatf& other)
    {
        float w_ = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
        float x_ = this->x * other.w + this->w * other.x + this->y * other.z - this->z * other.y;
        float y_ = this->y * other.w + this->w * other.y + this->z * other.x - this->x * other.z;
        float z_ = this->z * other.w + this->w * other.z + this->x * other.y - this->y * other.x;
        this->x = x_;
        this->y = y_;
        this->z = z_;
        this->w = w_;
        return *this;
    }

    Quatf Quatf::operator*(const Vec3f& other)
    {
        float w_ = -this->x * other.getX() - this->y * other.getY() - this->z * other.getZ();
        float x_ =  this->w * other.getX() + this->y * other.getZ() - this->z * other.getY();
        float y_ =  this->w * other.getY() + this->z * other.getX() - this->x * other.getZ();
        float z_ =  this->w * other.getZ() + this->x * other.getY() - this->y * other.getX();
        return Quatf(x_, y_, z_, w_);
    }

    Quatf Quatf::operator*=(const Vec3f& other)
    {
        float w_ = -this->x * other.getX() - this->y * other.getY() - this->z * other.getZ();
        float x_ =  this->w * other.getX() + this->y * other.getZ() - this->z * other.getY();
        float y_ =  this->w * other.getY() + this->z * other.getX() - this->x * other.getZ();
        float z_ =  this->w * other.getZ() + this->x * other.getY() - this->y * other.getX();
        this->x = x_;
        this->y = y_;
        this->z = z_;
        this->w = w_;
        return *this;
    }

    // -- Getters -- //
    float Quatf::getX() const
    {
        return this->x;
    }

    float Quatf::getY() const
    {
        return this->y;
    }

    float Quatf::getZ() const
    {
        return this->z;
    }

    float Quatf::getW() const
    {
        return this->w;
    }

    // -- Setters -- //
    void Quatf::setX(float x)
    {
        this->x = x;
    }

    void Quatf::setY(float y)
    {
        this->y = y;
    }

    void Quatf::setZ(float z)
    {
        this->z = z;
    }

    void Quatf::setW(float w)
    {
        this->w = w;
    }
}