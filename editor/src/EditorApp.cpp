#include <Ember.hpp>
#include <Ember/EntryPoint.hpp>

#include <EmberEditor/ContentBrowser.hpp>

class EditorApp : public Ember::Application
{
    private:
        RenderTexture view_texture;
        Camera2D editor_camera;
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
            Ember::Window::create("EditorApp", 1280, 720);
            rlImGuiSetup(true);
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            this->view_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
            this->editor_camera.zoom = 1.0f;
            this->editor_camera.target = { 0.0f, 0.0f };
            this->editor_camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
            this->editor_camera.rotation = 0.0f; 

            Ember::ContentBrowser::get_singleton().on_create("../../../sandbox");
        }

        void on_update(float delta) override
        {
            BeginTextureMode(this->view_texture);
                ClearBackground(BLACK);
                BeginMode2D(this->editor_camera);
                    DrawLineEx({ -10240.0f, 0.0f }, { 10240.0f, 0.0f }, 2, RED); // <-- X-AXIS
                    DrawLineEx({ 0.0f, -10240.0f }, { 0.0f, 10240.0f }, 2, GREEN); // <-- Y-AXIS
                EndMode2D();
            EndTextureMode();
        }

        void on_render() override
        {
            rlImGuiBegin();
                ImGui::DockSpaceOverViewport(0,  NULL, ImGuiDockNodeFlags_PassthruCentralNode);
                ImGui::Begin("Action Bar");
                    if(ImGui::Button("Play")) {

                    }
                ImGui::End();
                ImGui::Begin("Scene Tree");
                    ImGui::Text("TODO");
                ImGui::End();
                Ember::ContentBrowser::get_singleton().on_render();
                ImGui::Begin("Inspector");
                    ImGui::Text("TODO");
                ImGui::End();
                ImGui::Begin("2D Viewport");
                    rlImGuiImageRenderTextureFit(&this->view_texture, true);
                    if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
                        Vector2 delta = GetMouseDelta();
                        this->editor_camera.target.x -= delta.x / this->editor_camera.zoom;
                        this->editor_camera.target.y -= delta.y / this->editor_camera.zoom;
                    }
                ImGui::End();   
            rlImGuiEnd();
        }

        void on_shutdown() override
        {
            rlImGuiShutdown();
            Ember::Window::shutdown();
        }
};

Ember::Application* Ember::create_application()
{
    return new EditorApp();
}