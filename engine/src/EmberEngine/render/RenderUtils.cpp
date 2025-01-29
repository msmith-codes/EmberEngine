#include <EmberEngine/render/RenderUtils.hpp>
#include <EmberEngine/core/Window.hpp>

#include <glad/glad.h>

namespace EmberEngine
{
    void RenderUtils::clearScreen()
    {
        // TODO: Stencil buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderUtils::initGraphics()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        // TODO: Depth clamp for later

        glEnable(GL_FRAMEBUFFER_SRGB);
    }
}