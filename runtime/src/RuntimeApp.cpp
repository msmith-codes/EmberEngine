#include <Ember.hpp>
#include <Ember/EntryPoint.hpp>

class EditorApp : public Ember::Application
{
    public:
        EditorApp()
        {

        }

        ~EditorApp()
        {

        }

    protected:
        void on_create() override
        {
            Ember::Window::create("SandboxApp", 800, 600);
        }

        void on_update(float delta) override
        {

        }

        void on_render() override
        {
            DrawFPS(2, 2);
        }

        void on_shutdown() override
        {
            Ember::Window::shutdown();
        }
};

Ember::Application* Ember::create_application()
{
    return new EditorApp();
}