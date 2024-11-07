#include <Ember/Application.hpp>

// -- Ember -- //
#include <Ember/render/Window.hpp>
#include <Ember/render/Renderer.hpp>

// -- STD -- //
#include <chrono>
#include <thread>

namespace Ember
{
    // Static Variables
    Application* Application::instance = nullptr;

    Application::Application()
    {
        this->window = new Window();
        this->renderer = new Renderer();

        this->currentFPS = 0;

        Application::setInstance(this);
    }

    Application::~Application()
    {
        delete this->window;
        delete this->renderer;
    }

    void Application::setInstance(Application* app)
    {
        Application::instance = app;
    }

    void Application::run()
    {
        this->window->init();

        this->onStart();

        const unsigned int TARGET_FPS = this->window->getTargetFPS();
        const std::chrono::milliseconds FRAME_TIME(1000 / TARGET_FPS);

        auto lastTime = std::chrono::high_resolution_clock::now();

        unsigned int frameCount = 0;
        float fps = 0.0f;
        auto lastFPSTime = std::chrono::high_resolution_clock::now();

        while(!this->window->shouldWindowClose()) {
            // Start the timer
            auto startTime = std::chrono::high_resolution_clock::now();

            // Calculate delta time
            std::chrono::duration<float> delta = startTime - lastTime;
            lastTime = startTime;

            // Clear the screen
            this->renderer->prepareFrame();

            // Call the update method
            this->onUpdate(delta.count());
            
            // Call the render method
            this->onRender();

            // Swap the buffers and poll events
            this->window->swapBuffers();
            this->window->pollEvents();

            // Calculate the FPS
            frameCount++;

            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> fpsDelta = currentTime - lastFPSTime;

            if(fpsDelta.count() >= 1.0f) {
                fps = frameCount / fpsDelta.count();
                frameCount = 0;
                lastFPSTime = currentTime;
                this->currentFPS = fps;
            }

            // Sleep if needed
            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            if(elapsedTime < FRAME_TIME) {
                std::this_thread::sleep_for(FRAME_TIME - elapsedTime);
            }
        }
        this->onEnd();
        
        this->window->destroy();
    }

    Application* Application::getInstance()
    {
        if(Application::instance == nullptr) {
            Application::instance = createApplication();
        }
        return Application::instance;
    }

    unsigned int Application::getFPS()
    {
        return Application::getInstance()->currentFPS;
    }

    
}