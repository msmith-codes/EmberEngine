#include "Camera3D.hpp"

namespace EmberEngine
{
    Camera3D::Camera3D()
    {
        this->camera.position = { 0.0f, 10.0f, 10.0f };
        this->camera.target = { 0.0f, 0.0f, 0.0f };
        this->camera.up = { 0.0f, 1.0f, 0.0f };
        this->camera.fovy = 45.0f;
        this->camera.projection = CAMERA_PERSPECTIVE;
    }

    Camera3D::Camera3D(const ::Camera3D& camera)
        : camera(camera)
    {
    }

    ::Camera3D& Camera3D::raw()
    {
        return this->camera;
    }

    const ::Camera3D& Camera3D::raw() const
    {
        return this->camera;
    }

    void Camera3D::set_position(::Vector3 position)
    {
        this->camera.position = position;
    }

    void Camera3D::set_target(::Vector3 target)
    {
        this->camera.target = target;
    }

    void Camera3D::set_up(::Vector3 up)
    {
        this->camera.up = up;
    }

    void Camera3D::set_fovy(float fovy)
    {
        this->camera.fovy = fovy;
    }

    void Camera3D::set_projection(int projection)
    {
        this->camera.projection = projection;
    }

    ::Vector3 Camera3D::get_position() const
    {
        return this->camera.position;
    }

    ::Vector3 Camera3D::get_target() const
    {
        return this->camera.target;
    }

    ::Vector3 Camera3D::get_up() const
    {
        return this->camera.up;
    }

    float Camera3D::get_fovy() const
    {
        return this->camera.fovy;
    }

    int Camera3D::get_projection() const
    {
        return this->camera.projection;
    }

    Camera3D::operator ::Camera3D&()
    {
        return this->camera;
    }

    Camera3D::operator const ::Camera3D&() const
    {
        return this->camera;
    }
}
