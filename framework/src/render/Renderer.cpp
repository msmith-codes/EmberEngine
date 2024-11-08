#include <Ember/render/Renderer.hpp>

#include <glad/glad.h>

namespace Ember
{
    void Renderer::prepareFrame()
    {
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
}