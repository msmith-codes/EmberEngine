#include <EmberEngine/core/Application.hpp>
#include <EmberEngine/scene/SceneManager.hpp>

#include <imgui.h>
#include <rlImGui.h>
#include <raylib.h>

namespace EmberEngine
{
    void Application::run()
    {
        this->on_compose();

        rlImGuiSetup(true);

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        while(this->is_running()) {
            float delta = GetFrameTime();
            SceneManager::on_update(delta);
            this->on_update(delta);

            BeginDrawing();
            ClearBackground(BLACK);

            rlImGuiBegin();
            SceneManager::on_render();
            this->on_render();
            rlImGuiEnd();
            
            EndDrawing();
        }

        rlImGuiShutdown();

        this->on_dispose();
    }
}