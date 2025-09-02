#include <Ember/core/Application.hpp>
#include <Ember/core/Window.hpp>

#include <raylib.h>

namespace Ember
{
    Application* Application::instance = nullptr;

    // -- Constructor/Destructor -- //
    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    // -- Methods -- //
    void Application::run()
    {
        Application::get_singleton()->on_compose();

        while(!Window::should_close()) {
            float delta = GetFrameTime();
            Application::get_singleton()->on_update(delta);

            BeginDrawing();
                ClearBackground(BLACK);
                Application::get_singleton()->on_render();
            EndDrawing();
        }

        Application::get_singleton()->on_dispose();
    }

    Application* Application::get_singleton()
    {
        return Application::instance;
    }

    void Application::set_singleton(Application* instance)
    {
        if(!Application::instance) {
            Application::instance = instance;
        }
    }

    void Application::change_scene(Scene* scene)
    {
        if(!Application::instance) {
            return;
        }
        
        if(Application::instance->scene) {
            Application::instance->scene->on_dispose();
            delete Application::instance->scene;
            Application::instance->scene = nullptr;
        }

        Application::instance->scene = scene;
        Application::instance->scene->on_compose();
    }
}
