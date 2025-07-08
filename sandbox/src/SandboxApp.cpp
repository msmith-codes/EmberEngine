#include <Ember.hpp>

class SandboxApp : public Ember::Application
{
    public:
        SandboxApp()
        {

        }

        ~SandboxApp()
        {

        }

    protected:
        void on_create() override
        {

        }

        void on_update(float delta) override
        {

        }

        void on_render() override
        {

        }

        void on_shutdown() override
        {

        }
};

Ember::Application* Ember::create_application()
{
    return new SandboxApp();
}