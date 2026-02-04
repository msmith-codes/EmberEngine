#pragma once

#include <EmberEngine/core/Application.hpp>
extern EmberEngine::Application* EmberEngine::create_application();

#ifdef PLATFORM_DESKTOP
int main(int argc, char* argv[])
{
    EmberEngine::Application* app = EmberEngine::create_application();
    app->run();
    delete app;
    return 0;
}

#endif