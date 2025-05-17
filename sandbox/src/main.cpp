#include <Ember.hpp>

int main(int argc, char* argv[])
{
    Ember::Window::Create(1280, 720, "SandboxApp");

    while(!Ember::Window::ShouldClose()) {
        Ember::Window::PollEvents();

        Ember::Window::SwapBuffers();
    }

    Ember::Window::Shutdown();
    return 0;
}