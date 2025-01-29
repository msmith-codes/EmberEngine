#include <EmberEngine/math/Mat4f.hpp>

namespace EmberEngine
{
    Mat4f::Mat4f()
    {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                this->m[i][j] = 0.0f;
            }
        }
    }

    Mat4f::Mat4f(const Mat4f& other)
    {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                this->m[i][j] = other.m[i][j];
            }
        }
    }

    Mat4f& Mat4f::operator=(const Mat4f& other)
    {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                this->m[i][j] = other.m[i][j];
            }
        }
        return *this;
    }

    // -- Math -- //
    void Mat4f::initIdentity()
    {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                if(i == j) {
                    this->m[i][j] = 1.0f;
                } else {
                    this->m[i][j] = 0.0f;
                }
            }
        }
    }

    Mat4f Mat4f::operator*(const Mat4f& other)
    {
        Mat4f result;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                result.set(i, j,  this->m[i][0] * other.m[0][j] +
                                  this->m[i][1] * other.m[1][j] +
                                  this->m[i][2] * other.m[2][j] +
                                  this->m[i][3] * other.m[3][j]);
            }
        }
        return result;
    }

    // -- Getters -- //
    float* Mat4f::getMatrix()
    {
        return &this->m[0][0];
    }

    float Mat4f::get(int x, int y)
    {
        return this->m[x][y];
    }

    // -- Setters -- //
    void Mat4f::setMatrix(float* matrix)
    {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                this->m[i][j] = matrix[i * 4 + j];
            }
        }
    }

    void Mat4f::set(int x, int y, float value)
    {
        this->m[x][y] = value;
    }

}