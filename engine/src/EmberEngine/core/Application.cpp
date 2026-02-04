#include <EmberEngine/core/Application.hpp>

#include <raylib.h>

namespace EmberEngine
{
    void Application::run()
    {
        this->on_compose();

        while(this->is_running()) {
            float delta = GetFrameTime();
            this->on_update(delta);

            BeginDrawing();
                ClearBackground(BLACK);
                this->on_render();
            EndDrawing();
        }

        this->on_dispose();
    }
}