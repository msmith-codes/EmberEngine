#include "Ember/core/Application.hpp"
#include "Ember/core/Window.hpp"
#include "raylib.h"
#include "scenes/GameScene.hpp"
#include <Ember.hpp>
#include <Ember/EntryPoint.hpp>

class AsteroidsApp : public Ember::Application
{
    protected:
        void on_compose() override
        {
            Ember::Window::create("Asteriods", 800, 600);

            AsteroidsApp::change_scene(new GameScene());
        }

        void on_update(float delta) override
        {
            
        }

        void on_render() override
        {
            DrawFPS(2, 2);
        }

        void on_dispose() override
        {
            Ember::Window::shutdown();
        }
};

Ember::Application* Ember::create_application()
{
    return new AsteroidsApp();
}
