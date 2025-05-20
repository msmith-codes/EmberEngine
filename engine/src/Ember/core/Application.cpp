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

    Application* Application::getInstance()
    {
        return Application::instance;
    }

    void Application::setInstance(Application* instance)
    {
        if(!Application::instance) {
            Application::instance = instance;
        }
    }

    void Application::changeScene(const std::string& sceneName)
    {
        Application* self = Application::getInstance();
    }

    void Application::setDefaultScene(const std::string& sceneName)
    {

    }
}