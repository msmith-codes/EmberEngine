#include <Ember/core/Window.hpp>

#include <raylib.h>

namespace Ember
{
    Window& Window::get_singleton()
    {
        static Window instance;
        return instance;
    }

    Window::Window()
    {
        this->width = 800;
        this->height = 600;
        this->title = "EmberEngine";
    }

    Window::~Window()
    {

    }

    // -- Static Methods -- //
    void Window::create(const std::string& title, unsigned int width, unsigned int height)
    {
        Window& self = Window::get_singleton();
        self.title = title;
        self.width = width;
        self.height = height;

        InitWindow(self.width, self.height, self.title.c_str());
        SetExitKey(KEY_NULL);
    }

    bool Window::should_close()
    {
        return WindowShouldClose();
    }

    void Window::shutdown()
    {
        CloseWindow();
    }

    // -- Static Getters -- //
    std::string& Window::get_title()
    {
        return Window::get_singleton().title;
    }

    unsigned int Window::get_width()
    {
        return Window::get_singleton().width;
    }

    unsigned int Window::get_height()
    {
        return Window::get_singleton().height;
    }

    // -- Static Setters -- //
    void Window::set_title(const std::string& title)
    {
        Window& self = Window::get_singleton();
        self.title = title;

        if(IsWindowReady()) {
            SetWindowTitle(self.title.c_str());
        }
    }

    void Window::set_size(unsigned int width, unsigned int height)
    {
        Window& self = Window::get_singleton();
        self.width = width;
        self.height = height;

        if(IsWindowReady()) {
            SetWindowSize(self.width, self.height);
        }
    }
}