#include <Ember/core/Window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Ember
{
    // -- Singleton Accessor -- //
    Window& Window::GetInstance()
    {
        static Window Instance;
        return Instance;
    }

    // -- Constructor -- //
    Window::Window()
    {
        this->width = 800;
        this->height = 600;
        this->title = "EmberEngine";

        this->vsync = true;
    }

    // -- Destructor -- //
    Window::~Window() = default;

    // -- Static Methods -- //
    void Window::Create(unsigned int width, unsigned int height, const std::string& title)
    {
        Window& self = Window::GetInstance();

        self.width = width;
        self.height = height;
        self.title = title;

        if(!glfwInit()) {
            std::cerr << "Failed to initialize GLFW." << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Configure GLFW.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        self.window = glfwCreateWindow(self.width, self.height, self.title.c_str(), nullptr, nullptr);
        if(!self.window) {
            std::cerr << "Failed to create GLFW window." << std::endl;
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        if(primary) {
            const GLFWvidmode* vidMode = glfwGetVideoMode(primary);
            if(vidMode) {
                int monitorX;
                int monitorY;

                glfwGetMonitorWorkarea(primary, &monitorX, &monitorY, nullptr, nullptr);

                int xPos = monitorX + (vidMode->width - self.width) / 2;
                int yPos = monitorY + (vidMode->height - self.height) / 2;
                glfwSetWindowPos(self.window, xPos, yPos);
            }
        }

        glfwMakeContextCurrent(self.window);
        glfwSwapInterval(self.vsync); // <-- Enables VSync.

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize OpenGL loader\n";
            std::exit(EXIT_FAILURE);
        }
    }

    void Window::Shutdown()
    {
        Window& self = Window::GetInstance();
        if(self.window) {
            glfwDestroyWindow(self.window);
            glfwTerminate();
            self.window = nullptr;
        }
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers()
    {
        Window& self = Window::GetInstance();
        glfwSwapBuffers(self.window);
    }

    void Window::Clear()
    {
        Window& self = Window::GetInstance();

        int width;
        int height;

        glfwGetFramebufferSize(self.window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    bool Window::ShouldClose()
    {
        Window& self = Window::GetInstance();
        return glfwWindowShouldClose(self.window);
    }

}