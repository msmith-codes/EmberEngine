#include <Ember/EntryPoint.hpp>
#include <Ember/Ember.hpp>

class SandboxApp : public Ember::Application
{
    public:
        SandboxApp() : Ember::Application()
        {
            Ember::Logger::init(false, true);

            this->window->setTitle("Sandbox");   
            this->window->setScreenSize(1280, 720);
            this->window->setVsync(true);
            this->window->setFullscreen(false);
            this->window->setResizable(false);

            this->window->setTargetFPS(60);
        }
    public:
        void onStart() override
        {
            Ember::Logger::info("Hello, World!");
        }

        void onUpdate(float delta) override
        {
            if(Ember::KeyListener::isKeyJustPressed(Ember::KeyInput::KEY_SPACE)) {
                Ember::Logger::info("Space key was pressed!");
            }

            if(Ember::KeyListener::isKeyPressed(Ember::KeyInput::KEY_A)) {
                Ember::Logger::info("A key is being held down!");
            }
        }

        void onRender() override
        {
            
        }

        void onEnd() override
        {
            Ember::Logger::info("Goodbye, World!");
        }
};

// This is the entry point for the application.
Ember::Application* Ember::createApplication()
{
    Application* app = new SandboxApp(); // Create the application.
    // app->setInstance(app);
    return app;
}
