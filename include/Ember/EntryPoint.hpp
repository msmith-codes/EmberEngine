#ifndef EMBER_EntryPoint_HPP_
#define EMBER_EntryPoint_HPP_

#include <Ember/Application.hpp>

extern Ember::Application* Ember::createApplication();

int main(int argc, char** argv)
{
    Ember::Application* app = Ember::createApplication();
    app->run();
    delete app;
    return 0;
}

#endif 