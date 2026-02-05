#include "EditorViewport3D.hpp"
#include "EditorViewport2D.hpp"

#include <EmberEngine/EmberEngine.hpp>

#include <rlImGui.h>
#include <cmath>

EditorViewport3D::EditorViewport3D()
    : viewport_size(640, 480), is_hovered_(false), is_focused_(false),
      walls_ptr(nullptr), rooms_ptr(nullptr),
      camera_yaw(0.0f), camera_pitch(0.0f), 
      camera_position({ 0.0f, 32.0f, 0.0f }), movement_speed(50.0f)
{
    this->render_texture = ::LoadRenderTexture(static_cast<int>(this->viewport_size.x), 
                                              static_cast<int>(this->viewport_size.y));

    // Initialize camera for first-person
    this->camera.set_position(this->camera_position);
    this->camera.set_target({ 0.0f, 32.0f, 0.0f });
    this->camera.set_up({ 0.0f, 1.0f, 0.0f });
    this->camera.set_fovy(60.0f);
    this->camera.set_projection(CAMERA_PERSPECTIVE);
}

EditorViewport3D::~EditorViewport3D()
{
    // Unload all cached textures
    for(auto& pair : this->texture_cache) {
        ::UnloadTexture(pair.second);
    }
    this->texture_cache.clear();
    
    if(this->render_texture.id > 0) {
        ::UnloadRenderTexture(this->render_texture);
    }
}

void EditorViewport3D::begin_render()
{
    ::BeginTextureMode(this->render_texture);
    ::ClearBackground(::Color{ 20, 20, 30, 255 });
    EmberEngine::RenderEngine::begin_3d(this->camera);
}

void EditorViewport3D::end_render()
{
    // Draw grid on the floor
    ::DrawGrid(50, 16.0f);
    
    // Draw rooms first (floors and ceilings)
    this->render_rooms_3d();
    
    // Draw walls
    this->render_walls_3d();
    
    // Draw axis
    ::DrawLine3D({ 0, 0, 0 }, { 100, 0, 0 }, RED);
    ::DrawLine3D({ 0, 0, 0 }, { 0, 100, 0 }, GREEN);
    ::DrawLine3D({ 0, 0, 0 }, { 0, 0, 100 }, BLUE);
    
    EmberEngine::RenderEngine::end_3d();
    ::EndTextureMode();
}

void EditorViewport3D::render_ui()
{
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    if(ImGui::Begin("3D Viewport")) {
        ImGui::Text("Camera Controls: Right-click + WASD to move, Mouse to look around");
        ImGui::Separator();

        ImVec2 available_size = ImGui::GetContentRegionAvail();
        
        // Handle resize
        if(available_size.x != this->viewport_size.x || available_size.y != this->viewport_size.y) {
            if(available_size.x > 0 && available_size.y > 0) {
                ::UnloadRenderTexture(this->render_texture);
                this->viewport_size = available_size;
                this->render_texture = ::LoadRenderTexture(static_cast<int>(this->viewport_size.x), 
                                                           static_cast<int>(this->viewport_size.y));
            }
        }

        // Display the rendered texture
        rlImGuiImageRenderTexture(&this->render_texture);

        // Track hover and focus state
        this->is_hovered_ = ImGui::IsItemHovered();
        this->is_focused_ = ImGui::IsWindowFocused();

        // Handle input
        this->handle_input();
    }
    ImGui::End();
}

void EditorViewport3D::set_walls(const std::vector<Wall>* walls)
{
    this->walls_ptr = walls;
}

void EditorViewport3D::set_rooms(const std::vector<Room>* rooms)
{
    this->rooms_ptr = rooms;
}

EmberEngine::Camera3D& EditorViewport3D::get_camera()
{
    return this->camera;
}

const EmberEngine::Camera3D& EditorViewport3D::get_camera() const
{
    return this->camera;
}

bool EditorViewport3D::is_hovered() const
{
    return this->is_hovered_;
}

bool EditorViewport3D::is_focused() const
{
    return this->is_focused_;
}

void EditorViewport3D::render_walls_3d()
{
    if(!this->walls_ptr) return;
    
    for(const auto& wall : *this->walls_ptr) {
        // Calculate wall direction and perpendicular
        ::Vector2 dir = { wall.end.x - wall.start.x, wall.end.y - wall.start.y };
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        
        if(length < 0.001f) continue;
        
        // Draw wall as a vertical quad
        ::Vector3 p1 = { wall.start.x, 0.0f, wall.start.y };
        ::Vector3 p2 = { wall.end.x, 0.0f, wall.end.y };
        ::Vector3 p3 = { wall.end.x, wall.height, wall.end.y };
        ::Vector3 p4 = { wall.start.x, wall.height, wall.start.y };
        
        // Check if wall has texture
        if(!wall.texture_path.empty()) {
            ::Texture2D texture = this->get_or_load_texture(wall.texture_path);
            if(texture.id > 0) {
                // Draw textured quad (simplified without proper UV mapping)
                // For now, just draw with white color to indicate texture is loaded
                ::DrawTriangle3D(p1, p2, p3, WHITE);
                ::DrawTriangle3D(p1, p3, p4, WHITE);
                
                // Draw back face
                ::DrawTriangle3D(p2, p1, p4, ::Color{ 200, 200, 200, 255 });
                ::DrawTriangle3D(p2, p4, p3, ::Color{ 200, 200, 200, 255 });
            }
        } else {
            // Draw solid color wall
            ::Color wall_color = wall.is_portal ? ::Color{ 120, 200, 255, 255 } : ::Color{ 180, 180, 180, 255 };
            ::DrawTriangle3D(p1, p2, p3, wall_color);
            ::DrawTriangle3D(p1, p3, p4, wall_color);
            
            // Draw back face
            ::Color back_color = wall.is_portal ? ::Color{ 90, 170, 220, 255 } : ::Color{ 160, 160, 160, 255 };
            ::DrawTriangle3D(p2, p1, p4, back_color);
            ::DrawTriangle3D(p2, p4, p3, back_color);
        }
        
        // Draw edges
        ::DrawLine3D(p1, p2, WHITE);
        ::DrawLine3D(p2, p3, WHITE);
        ::DrawLine3D(p3, p4, WHITE);
        ::DrawLine3D(p4, p1, WHITE);
        
        // Draw height markers
        ::DrawSphere({ wall.start.x, wall.height, wall.start.y }, 2.0f, YELLOW);
        ::DrawSphere({ wall.end.x, wall.height, wall.end.y }, 2.0f, YELLOW);
    }
}

void EditorViewport3D::render_rooms_3d()
{
    if(!this->rooms_ptr) return;
    
    for(const auto& room : *this->rooms_ptr) {
        if(room.vertices.size() < 3) continue;
        
        // Draw floor
        ::Color floor_color = room.color;
        floor_color.a = 200;
        ::Texture2D floor_texture = { 0 };
        if(!room.floor_texture_path.empty()) {
            floor_texture = this->get_or_load_texture(room.floor_texture_path);
        }
        
        for(size_t i = 1; i < room.vertices.size() - 1; ++i) {
            ::Vector3 v1 = { room.vertices[0].x, room.floor_height, room.vertices[0].y };
            ::Vector3 v2 = { room.vertices[i].x, room.floor_height, room.vertices[i].y };
            ::Vector3 v3 = { room.vertices[i + 1].x, room.floor_height, room.vertices[i + 1].y };
            
            if(floor_texture.id > 0) {
                // Draw with texture (simplified - actual UV mapping would be more complex)
                ::DrawTriangle3D(v1, v2, v3, WHITE);
            } else {
                ::DrawTriangle3D(v1, v2, v3, floor_color);
            }
            ::Color floor_darker = {
                static_cast<unsigned char>(floor_color.r > 10 ? floor_color.r - 10 : 0),
                static_cast<unsigned char>(floor_color.g > 10 ? floor_color.g - 10 : 0),
                static_cast<unsigned char>(floor_color.b > 10 ? floor_color.b - 10 : 0),
                floor_color.a
            };
            ::DrawTriangle3D(v1, v3, v2, floor_darker);
        }
        
        // Draw ceiling
        ::Color ceiling_color = room.color;
        ceiling_color.a = 200;
        ::Texture2D ceiling_texture = { 0 };
        if(!room.ceiling_texture_path.empty()) {
            ceiling_texture = this->get_or_load_texture(room.ceiling_texture_path);
        }
        
        for(size_t i = 1; i < room.vertices.size() - 1; ++i) {
            ::Vector3 v1 = { room.vertices[0].x, room.ceiling_height, room.vertices[0].y };
            ::Vector3 v2 = { room.vertices[i].x, room.ceiling_height, room.vertices[i].y };
            ::Vector3 v3 = { room.vertices[i + 1].x, room.ceiling_height, room.vertices[i + 1].y };
            
            if(ceiling_texture.id > 0) {
                ::DrawTriangle3D(v1, v3, v2, WHITE);
            } else {
                ::DrawTriangle3D(v1, v3, v2, ceiling_color);
            }
            ::Color ceiling_darker = {
                static_cast<unsigned char>(ceiling_color.r > 10 ? ceiling_color.r - 10 : 0),
                static_cast<unsigned char>(ceiling_color.g > 10 ? ceiling_color.g - 10 : 0),
                static_cast<unsigned char>(ceiling_color.b > 10 ? ceiling_color.b - 10 : 0),
                ceiling_color.a
            };
            ::DrawTriangle3D(v1, v2, v3, ceiling_darker);
        }
    }
}

void EditorViewport3D::handle_input()
{
    if(!this->is_hovered_) {
        return;
    }

    bool is_right_clicking = ImGui::IsMouseDown(ImGuiMouseButton_Right);
    static bool was_right_clicking = false;
    
    // Hide and lock cursor when right-click starts
    if(is_right_clicking && !was_right_clicking) {
        ::DisableCursor();
    }
    // Show cursor when right-click ends
    else if(!is_right_clicking && was_right_clicking) {
        ::EnableCursor();
    }
    
    was_right_clicking = is_right_clicking;
    
    // Mouse look when right-clicking - use GetMouseDelta for locked cursor
    if(is_right_clicking) {
        ::Vector2 mouse_delta = ::GetMouseDelta();
        
        if(mouse_delta.x != 0.0f || mouse_delta.y != 0.0f) {
            this->camera_yaw += mouse_delta.x * 0.2f;
            this->camera_pitch -= mouse_delta.y * 0.2f;
            
            // Clamp pitch to prevent camera flip
            if(this->camera_pitch > 89.0f) this->camera_pitch = 89.0f;
            if(this->camera_pitch < -89.0f) this->camera_pitch = -89.0f;
            
            this->update_camera_target();
        }
    }

    // WASD movement when right-clicking
    if(is_right_clicking) {
        float delta_time = ::GetFrameTime();
        float move_speed = this->movement_speed * delta_time;
        
        // Calculate forward and right vectors
        float yaw_rad = this->camera_yaw * 3.14159265f / 180.0f;
        ::Vector3 forward = {
            std::sin(yaw_rad),
            0.0f,
            std::cos(yaw_rad)
        };
        ::Vector3 right = {
            std::cos(yaw_rad),
            0.0f,
            -std::sin(yaw_rad)
        };
        
        // WASD movement
        if(ImGui::IsKeyDown(ImGuiKey_W)) {
            this->camera_position.x += forward.x * move_speed;
            this->camera_position.z += forward.z * move_speed;
        }
        if(ImGui::IsKeyDown(ImGuiKey_S)) {
            this->camera_position.x -= forward.x * move_speed;
            this->camera_position.z -= forward.z * move_speed;
        }
        if(ImGui::IsKeyDown(ImGuiKey_A)) {
            this->camera_position.x -= right.x * move_speed;
            this->camera_position.z -= right.z * move_speed;
        }
        if(ImGui::IsKeyDown(ImGuiKey_D)) {
            this->camera_position.x += right.x * move_speed;
            this->camera_position.z += right.z * move_speed;
        }
        
        // Vertical movement
        if(ImGui::IsKeyDown(ImGuiKey_Space)) {
            this->camera_position.y += move_speed;
        }
        if(ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
            this->camera_position.y -= move_speed;
        }
        
        this->camera.set_position(this->camera_position);
        this->update_camera_target();
    }
}

void EditorViewport3D::update_camera_target()
{
    float yaw_rad = this->camera_yaw * 3.14159265f / 180.0f;
    float pitch_rad = this->camera_pitch * 3.14159265f / 180.0f;
    
    // Calculate look direction
    ::Vector3 direction = {
        std::cos(pitch_rad) * std::sin(yaw_rad),
        std::sin(pitch_rad),
        std::cos(pitch_rad) * std::cos(yaw_rad)
    };
    
    // Set target to be in front of camera
    ::Vector3 target = {
        this->camera_position.x + direction.x,
        this->camera_position.y + direction.y,
        this->camera_position.z + direction.z
    };
    
    this->camera.set_target(target);
}

::Texture2D EditorViewport3D::get_or_load_texture(const std::string& path)
{
    // Check if texture is already loaded
    auto it = this->texture_cache.find(path);
    if(it != this->texture_cache.end()) {
        return it->second;
    }
    
    // Try to load the texture
    ::Texture2D texture = ::LoadTexture(path.c_str());
    
    // Cache it (even if failed, to avoid repeated load attempts)
    this->texture_cache[path] = texture;
    
    return texture;
}
