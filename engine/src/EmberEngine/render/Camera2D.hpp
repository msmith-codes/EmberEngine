#pragma once

#include <raylib.h>

namespace EmberEngine
{
    class Camera2D
    {
        public:
            Camera2D();
            explicit Camera2D(const ::Camera2D& camera);

            ::Camera2D& raw();
            const ::Camera2D& raw() const;

            void set_target(::Vector2 target);
            void set_offset(::Vector2 offset);
            void set_rotation(float rotation);
            void set_zoom(float zoom);

            ::Vector2 get_target() const;
            ::Vector2 get_offset() const;
            float get_rotation() const;
            float get_zoom() const;

            operator ::Camera2D&();
            operator const ::Camera2D&() const;

        private:
            ::Camera2D camera;
    };
}