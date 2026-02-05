#pragma once

#include <raylib.h>

namespace EmberEngine
{
    class Camera3D
    {
        public:
            Camera3D();
            explicit Camera3D(const ::Camera3D& camera);

            ::Camera3D& raw();
            const ::Camera3D& raw() const;

            void set_position(::Vector3 position);
            void set_target(::Vector3 target);
            void set_up(::Vector3 up);
            void set_fovy(float fovy);
            void set_projection(int projection);

            ::Vector3 get_position() const;
            ::Vector3 get_target() const;
            ::Vector3 get_up() const;
            float get_fovy() const;
            int get_projection() const;

            operator ::Camera3D&();
            operator const ::Camera3D&() const;

        private:
            ::Camera3D camera;
    };
}
