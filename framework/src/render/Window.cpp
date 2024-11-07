#include <Ember/render/Window.hpp>

// -- Custom -- //
#include <Ember/utils/Logger.hpp>
#include <Ember/utils/Helper.hpp>
#include <Ember/input/KeyListener.hpp>

// -- STD -- //

// -- GLAD -- //
#include <glad/glad.h>

// -- GLFW -- //
#include <GLFW/glfw3.h>

namespace Ember
{
    // Constructor & Destructor
    //-------------------------------------------------------------------------
    Window::Window()
    {
        this->width = 800;
        this->height = 600;
        this->title = "EmberEngine";
        this->fullscreen = false;
        this->resizable = true;
        this->vsync = true;
        this->targetFPS = 60;
    }

    Window::~Window()
    {
        this->destroy();
    }

    // Setters
    //-------------------------------------------------------------------------
    void Window::setScreenSize(unsigned int width, unsigned int height)
    {
        this->width = width;
        this->height = height;
    }

    void Window::setTitle(const std::string& title)
    {
        this->title = title;
    }

    void Window::setFullscreen(bool fullscreen)
    {
        this->fullscreen = fullscreen;
    }

    void Window::setResizable(bool resizable)
    {
        this->resizable = resizable;
    }

    void Window::setVsync(bool vsync)
    {
        this->vsync = vsync;
    }

    void Window::setTargetFPS(unsigned int targetFPS)
    {
        this->targetFPS = targetFPS;
    }

    // Getters
    //-------------------------------------------------------------------------
    unsigned int Window::getWidth() const
    {
        return this->width;
    }

    unsigned int Window::getHeight() const
    {
        return this->height;
    }

    const std::string& Window::getTitle() const
    {
        return this->title;
    }

    bool Window::isFullscreen() const
    {
        return this->fullscreen;
    }

    bool Window::isResizable() const
    {
        return this->resizable;
    }

    bool Window::isVsync() const
    {
        return this->vsync;
    }

    unsigned int Window::getTargetFPS() const
    {
        return this->targetFPS;
    }

    unsigned int Window::getFPS() const
    {
        return 0;
    }

    // Methods
    //-------------------------------------------------------------------------
    void Window::init()
    {
        // Initialize GLFW
        if(!glfwInit()) {
            Logger::fatal("Failed to initialize GLFW");
            exit(EXIT_FAILURE);
        }

        // Set the window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, this->resizable);
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Create the window for fullscreen or not
        if(this->fullscreen) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            this->width = mode->width;
            this->height = mode->height;
            this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), monitor, nullptr);
        } else {
            this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), nullptr, nullptr);
        }

        // Check if the window failed to create
        if(this->window == nullptr) {
            Logger::fatal("Failed to create GLFW window");
            exit(EXIT_FAILURE);
        }

        // Register input callbacks
        glfwSetKeyCallback(this->window, KeyListener::keyCallback);

        // Make the window's context current
        glfwMakeContextCurrent(this->window);

        // Load GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Logger::fatal("Failed to initialize GLAD");
            exit(EXIT_FAILURE);
        }

        // Set the Vsync
        glfwSwapInterval(this->vsync);

        // Log the window creation
        Logger::debug("Window created successfully");
    }

    bool Window::shouldWindowClose()
    {
        return glfwWindowShouldClose(this->window);
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(this->window);
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    void Window::destroy()
    {
        if(this->window != nullptr) {
            glfwDestroyWindow(this->window);
            this->window = nullptr;
            glfwTerminate();
        }
    }
}