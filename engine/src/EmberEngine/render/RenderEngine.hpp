#pragma once

#include <EmberEngine/render/Camera2D.hpp>
#include <EmberEngine/render/Camera3D.hpp>

#include <raylib.h>

namespace EmberEngine
{
    class RenderEngine
    {
        public:
            static void draw_fps(unsigned int x, unsigned int y);
        public:
            static void begin_2d(const Camera2D& camera);
            static void end_2d();
            
            static void begin_3d(const Camera3D& camera);
            static void end_3d();
    };
}