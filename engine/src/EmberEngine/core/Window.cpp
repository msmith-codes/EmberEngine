#include <EmberEngine/core/Window.hpp>

#include <EmberEngine/input/MouseInput.hpp>
#include <EmberEngine/input/KeyInput.hpp>

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace EmberEngine
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
        this->resizable = false;
    }

    // -- Getters -- //
    int Window::getWidth()
    {
        return getInstance().width;
    }

    int Window::getHeight()
    {
        return getInstance().height;
    }

    std::string Window::getTitle()
    {
        return getInstance().title;
    }

    bool Window::getResizable()
    {
        return getInstance().resizable;
    }

    // -- Setters -- //
    void Window::setSize(const int width, const int height)
    {
        getInstance().width = width;
        getInstance().height = height;
    }

    void Window::setTitle(const std::string& title)
    {
        getInstance().title = title;
    }

    void Window::setResizable(const bool resizable)
    {
        getInstance().resizable = resizable;
    }

    // -- Static Methods -- //
    void Window::compose()
    {
        Window& win = Window::getInstance();
        if(!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            std::exit(-1);
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if(win.resizable) {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        } else {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        }

        win.window = glfwCreateWindow(win.width, win.height, win.title.c_str(), nullptr, nullptr);

        if(!win.window) {
            std::cerr << "Failed to create window" << std::endl;
            glfwTerminate();
            std::exit(-1);
        }

        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if(primaryMonitor) {
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            if(mode) {
                int xpos = (mode->width - win.width) / 2;
                int ypos = (mode->height - win.height) / 2;
                glfwSetWindowPos(win.window, xpos, ypos);
            }
        }

        glfwSetCursorPosCallback(win.window, MouseInput::mousePosCallback);
        glfwSetMouseButtonCallback(win.window, MouseInput::mouseButtonCallback);
        glfwSetScrollCallback(win.window, MouseInput::mouseScrollCallback);

        glfwSetKeyCallback(win.window, KeyInput::keyCallback);

        glfwMakeContextCurrent(win.window);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            std::exit(-1);
        }

    }

    void Window::dispose()
    {
        Window& win = Window::getInstance();
        glfwDestroyWindow(win.window);
        glfwTerminate();
    }

    void Window::swapBuffers()
    {
        Window& win = Window::getInstance();
        glfwSwapBuffers(win.window);
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    bool Window::isOpen()
    {
        Window& win = Window::getInstance();
        return !glfwWindowShouldClose(win.window);
    }
}