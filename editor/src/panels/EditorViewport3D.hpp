#pragma once

#include <EmberEngine/render/Camera3D.hpp>
#include <imgui.h>
#include <raylib.h>
#include <vector>
#include <map>
#include <string>

// Forward declarations
struct Wall;
struct Room;

class EditorViewport3D
{
    public:
        EditorViewport3D();
        ~EditorViewport3D();

        void begin_render();
        void end_render();
        void render_ui();
        
        void set_walls(const std::vector<Wall>* walls);
        void set_rooms(const std::vector<Room>* rooms);
        
        EmberEngine::Camera3D& get_camera();
        const EmberEngine::Camera3D& get_camera() const;
        
        bool is_hovered() const;
        bool is_focused() const;

    private:
        EmberEngine::Camera3D camera;
        ::RenderTexture2D render_texture;
        ImVec2 viewport_size;
        bool is_hovered_;
        bool is_focused_;
        
        const std::vector<Wall>* walls_ptr;
        const std::vector<Room>* rooms_ptr;
        
        float camera_yaw;
        float camera_pitch;
        ::Vector3 camera_position;
        float movement_speed;
        
        std::map<std::string, ::Texture2D> texture_cache;

        void render_walls_3d();
        void render_rooms_3d();
        void handle_input();
        void update_camera_target();
        ::Texture2D get_or_load_texture(const std::string& path);
};
