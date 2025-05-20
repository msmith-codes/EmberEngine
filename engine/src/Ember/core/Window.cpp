#include <Ember/core/Window.hpp>

#include <raylib.h>

namespace Ember
{
    Window& Window::getInstance()
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
        Window& self = Window::getInstance();
        self.title = title;
        self.width = width;
        self.height = height;

        InitWindow(self.width, self.height, self.title.c_str());
        SetExitKey(KEY_NULL);
    }

    bool Window::shouldClose()
    {
        return WindowShouldClose();
    }

    void Window::shutdown()
    {
        CloseWindow();
    }

    // -- Static Getters -- //
    std::string& Window::getTitle()
    {
        return Window::getInstance().title;
    }

    unsigned int Window::getWidth()
    {
        return Window::getInstance().width;
    }

    unsigned int Window::getHeight()
    {
        return Window::getInstance().height;
    }

    // -- Static Setters -- //
    void Window::setTitle(const std::string& title)
    {
        Window& self = Window::getInstance();
        self.title = title;

        if(IsWindowReady()) {
            SetWindowTitle(self.title.c_str());
        }
    }

    void Window::setSize(unsigned int width, unsigned int height)
    {
        Window& self = Window::getInstance();
        self.width = width;
        self.height = height;

        if(IsWindowReady()) {
            SetWindowSize(self.width, self.height);
        }
    }
}