#include <Ember/core/Application.hpp>

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

    void Application::change_scene(const std::string& sceneName)
    {
        Application* self = Application::get_singleton();
    }

    void Application::set_default_scene(const std::string& sceneName)
    {

    }
}