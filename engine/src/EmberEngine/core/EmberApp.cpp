#include <EmberEngine/core/EmberApp.hpp>

#include <EmberEngine/core/Time.hpp>
#include <EmberEngine/core/Window.hpp>
#include <EmberEngine/render/RenderUtils.hpp>

namespace EmberEngine
{
    EmberApp::EmberApp()
    {
    }

    void EmberApp::run()
    {
        Window::compose();
        RenderUtils::initGraphics();

        float beginTime = Time::getTime();
        float endTime = Time::getTime();

        float dt = -1.0f;

        this->onCompose();

        while(Window::isOpen()) {
            Window::pollEvents();

            if(dt >= 0.0f) {
                this->onUpdate(dt);
            }
            
            this->onRender();

            endTime = Time::getTime();
            dt = endTime - beginTime;
            beginTime = endTime;

            Window::swapBuffers();
        }

        Window::dispose();
        this->onDispose();
    }
}