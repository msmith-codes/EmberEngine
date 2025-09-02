#pragma once

#include <Ember/core/Application.hpp>

extern Ember::Application* Ember::create_application();

#if defined(EE_PLATFORM_MACOS) || defined(EE_PLATFORM_LINUX) || defined(EE_PLATFORM_WINDOWS)

int main(int argc, char* argv[])
{
    auto app = Ember::create_application();
    Ember::Application::set_singleton(app);
    app->run();
    delete app;
    return 0;
}

#endif

