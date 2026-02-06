#pragma once

#include <EmberEngine/render/Camera2D.hpp>
#include <imgui.h>
#include <raylib.h>
#include <vector>
#include <string>

struct Wall
{
    ::Vector2 start;
    ::Vector2 end;
    float height;
    std::string texture_path;
    bool is_portal;
    int portal_room_a;
    int portal_room_b;
    
    Wall(::Vector2 s, ::Vector2 e, float h = 64.0f)
    {
        this->start = s;
        this->end = e;
        this->height = h;
        this->texture_path = "";
        this->is_portal = false;
        this->portal_room_a = -1;
        this->portal_room_b = -1;
    }
};

struct Room
{
    std::vector<::Vector2> vertices;
    float floor_height;
    float ceiling_height;
    std::string floor_texture_path;
    std::string ceiling_texture_path;
    ::Color color;
    
        Room()
                : floor_height(0.0f), ceiling_height(128.0f),
                    floor_texture_path(""), ceiling_texture_path(""),
                    color({ 100, 100, 100, 80 })
    {
    }
};

enum class EditorMode
{
    Select,
    PlaceWall
};

class EditorViewport2D
{
    public:
        EditorViewport2D();
        ~EditorViewport2D();

        void begin_render();
        void end_render();
        void render_ui();

        EmberEngine::Camera2D& get_camera();
        const EmberEngine::Camera2D& get_camera() const;

        bool is_hovered() const;
        bool is_focused() const;

        ::Vector2 get_viewport_size() const;
        ::Vector2 get_viewport_position() const;
        
        void set_grid_size(float size);
        float get_grid_size() const;

        void add_wall(const Wall& wall);
        void clear_walls();
        const std::vector<Wall>& get_walls() const;
        std::vector<Wall>& get_walls_mutable();
        
        // Map loading
        bool load_map(const std::string& filepath);
        
        // Current map file tracking
        const std::string& get_current_map_path() const;
        std::string get_current_map_filename() const;
        bool has_map_loaded() const;
        
        int get_selected_wall_index() const;
        void set_selected_wall_index(int index);
        bool has_selection() const;
        
        const std::vector<Room>& get_rooms() const;
        std::vector<Room>& get_rooms_mutable();
        int get_selected_room_index() const;
        void set_selected_room_index(int index);
        bool has_room_selection() const;
        void detect_rooms();

    private:
        EmberEngine::Camera2D camera;
        ::RenderTexture2D render_texture;
        ImVec2 viewport_size;
        float grid_size;
        bool is_hovered_;
        bool is_focused_;

        std::string current_map_path;  // Path of currently loaded map file
        
        EditorMode current_mode;
        std::vector<Wall> walls;
        std::vector<Room> rooms;
        bool is_placing_wall;
        ::Vector2 wall_start_pos;
        ::Vector2 current_mouse_world_pos;
        int selected_wall_index;
        int selected_room_index;
        
        bool is_dragging_vertex;
        int dragging_wall_index;
        bool dragging_start_vertex; // true for start, false for end

        void render_grid();
        void render_walls();
        void render_rooms();
        void handle_input();
        ::Vector2 screen_to_world(ImVec2 screen_pos);
        ::Vector2 snap_to_grid(::Vector2 pos);
        float distance_to_line(::Vector2 point, ::Vector2 line_start, ::Vector2 line_end);
        float distance_to_point(::Vector2 p1, ::Vector2 p2);
        bool point_in_polygon(::Vector2 point, const std::vector<::Vector2>& polygon);
};