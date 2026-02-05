#include <EmberEngine/render/Camera2D.hpp>

namespace EmberEngine
{
    Camera2D::Camera2D()
    {
        this->camera = { 0 };
        this->camera.target = { 0.0f, 0.0f };
        this->camera.offset = { 0.0f, 0.0f };
        this->camera.rotation = 0.0f;
        this->camera.zoom = 1.0f;
    }

    Camera2D::Camera2D(const ::Camera2D& camera)
    {
        this->camera = camera;
    }

    ::Camera2D& Camera2D::raw()
    {
        return this->camera;
    }

    const ::Camera2D& Camera2D::raw() const
    {
        return this->camera;
    }

    void Camera2D::set_target(::Vector2 target)
    {
        this->camera.target = target;
    }

    void Camera2D::set_offset(::Vector2 offset)
    {
        this->camera.offset = offset;
    }

    void Camera2D::set_rotation(float rotation)
    {
        this->camera.rotation = rotation;
    }

    void Camera2D::set_zoom(float zoom)
    {
        this->camera.zoom = zoom;
    }

    ::Vector2 Camera2D::get_target() const
    {
        return this->camera.target;
    }

    ::Vector2 Camera2D::get_offset() const
    {
        return this->camera.offset;
    }

    float Camera2D::get_rotation() const
    {
        return this->camera.rotation;
    }

    float Camera2D::get_zoom() const
    {
        return this->camera.zoom;
    }

    Camera2D::operator ::Camera2D&()
    {
        return this->camera;
    }

    Camera2D::operator const ::Camera2D&() const
    {
        return this->camera;
    }
}

