#include <EmberEngine/render/RenderEngine.hpp>

#include <raylib.h>

namespace EmberEngine
{
    void RenderEngine::draw_fps(unsigned int x, unsigned int y)
    {
        ::DrawFPS(x, y);
    }

    void RenderEngine::begin_2d(const Camera2D& camera)
    {
        ::BeginMode2D(camera);
    }

    void RenderEngine::end_2d()
    {
        ::EndMode2D();
    }

    void RenderEngine::begin_3d(const Camera3D& camera)
    {
        ::BeginMode3D(camera);
    }

    void RenderEngine::end_3d()
    {
        ::EndMode3D();
    }
}