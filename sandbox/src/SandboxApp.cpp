#include <EmberEngine/EmberEngine.hpp>
#include <EmberEngine/EntryPoint.hpp>

class SandboxApp : public EmberEngine::Application
{
    protected:
        void on_compose() override
        {
            EmberEngine::Display::create(800, 600, "SandboxApp");
            EmberEngine::Display::set_target_fps(60);
        }

        void on_dispose() override
        {
            EmberEngine::Display::destroy();
        }

        void on_update(float delta) override
        {

        }

        void on_render() override
        {
            ::DrawFPS(2, 2);
        }

        bool is_running() override
        {
            return (!EmberEngine::Display::should_close());
        }

};

EmberEngine::Application* EmberEngine::create_application()
{
    return new SandboxApp();
}