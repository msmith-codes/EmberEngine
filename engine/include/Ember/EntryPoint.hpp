#ifndef EE_EntryPoint_HPP
#define EE_EntryPoint_HPP

#include <Ember/core/Application.hpp>

extern Ember::Application* Ember::create_application();

#if defined(EE_PLATFORM_MACOS)

int main(int argc, char* argv[])
{
    auto app = Ember::create_application();
    Ember::Application::set_singleton(app);
    app->run();
    delete app;
    return 0;
}

#endif

#endif