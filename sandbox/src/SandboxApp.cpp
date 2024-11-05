#include <Ember/EntryPoint.hpp>
#include <Ember/Ember.hpp>

class SandboxApp : public Ember::Application
{
    public:
        SandboxApp() : Ember::Application()
        {
            Ember::Logger::init(false, true);
        }
    public:
        void onStart() override
        {
            Ember::Logger::info("Hello, World!");
        }
        void onUpdate(float delta) override
        {
            // This will be called every frame.
        }
        void onRender() override
        {
            // This will be called every frame.
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
    app->setInstance(app);
    return app;
}
