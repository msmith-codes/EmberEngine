#include "EditorViewport2D.hpp"

#include <EmberEngine/EmberEngine.hpp>

#include <LevelSerializer.hpp>

#include <rlImGui.h>
#include <algorithm>
#include <cmath>
#include <cstring>

// Grid size selection buttons
EditorViewport2D::EditorViewport2D()
    : viewport_size(640, 480), grid_size(16.0f), is_hovered_(false), is_focused_(false),
      current_mode(EditorMode::Select), is_placing_wall(false), 
      wall_start_pos({0, 0}), current_mouse_world_pos({0, 0}), selected_wall_index(-1),
      selected_room_index(-1), is_dragging_vertex(false), dragging_wall_index(-1), 
      dragging_start_vertex(true)
{
    this->map_path_buffer[0] = '\0';
    std::strncpy(this->map_path_buffer, "level.map", sizeof(this->map_path_buffer) - 1);
    this->map_path_buffer[sizeof(this->map_path_buffer) - 1] = '\0';

    this->render_texture = ::LoadRenderTexture(static_cast<int>(this->viewport_size.x), 
                                              static_cast<int>(this->viewport_size.y));

    // Initialize camera to center of viewport
    this->camera.set_target({ 0.0f, 0.0f });
    this->camera.set_offset({ this->viewport_size.x * 0.5f, this->viewport_size.y * 0.5f });
    this->camera.set_zoom(1.0f);
}

EditorViewport2D::~EditorViewport2D()
{
    if(this->render_texture.id > 0) {
        ::UnloadRenderTexture(this->render_texture);
    }
}

void EditorViewport2D::begin_render()
{
    ::BeginTextureMode(this->render_texture);
    ::ClearBackground(BLACK);
    EmberEngine::RenderEngine::begin_2d(this->camera);
}

void EditorViewport2D::end_render()
{
    // Draw the grid
    this->render_grid();
    
    // Draw rooms
    this->render_rooms();
    
    // Draw walls
    this->render_walls();
    
    // Draw wall being placed
    if(this->is_placing_wall && this->current_mode == EditorMode::PlaceWall) {
        ::DrawLineEx(this->wall_start_pos, this->current_mouse_world_pos, 3.0f, YELLOW);
        ::DrawCircleV(this->wall_start_pos, 4.0f, GREEN);
    }
    
    // Draw the xy axis lines
    ::DrawLine(-10240, 0, 10240, 0, ::RED);
    ::DrawLine(0, -10240, 0, 10240, ::GREEN);
    
    EmberEngine::RenderEngine::end_2d();
    ::EndTextureMode();
}

void EditorViewport2D::render_ui()
{
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    if(ImGui::Begin("2D Viewport")) {
        // Mode selection
        ImGui::Text("Mode:");
        ImGui::SameLine();
        if(ImGui::RadioButton("Select", this->current_mode == EditorMode::Select)) {
            this->current_mode = EditorMode::Select;
            this->is_placing_wall = false;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Place Wall", this->current_mode == EditorMode::PlaceWall)) {
            this->current_mode = EditorMode::PlaceWall;
        }
        
        ImGui::SameLine();
        ImGui::Text("|");
        ImGui::SameLine();
        
        // Grid size controls
        ImGui::Text("Grid Size:");
        ImGui::SameLine();
        if(ImGui::Button("1##grid", ImVec2(30, 0))) { this->grid_size = 1.0f; }
        ImGui::SameLine();
        if(ImGui::Button("2##grid", ImVec2(30, 0))) { this->grid_size = 2.0f; }
        ImGui::SameLine();
        if(ImGui::Button("4##grid", ImVec2(30, 0))) { this->grid_size = 4.0f; }
        ImGui::SameLine();
        if(ImGui::Button("8##grid", ImVec2(30, 0))) { this->grid_size = 8.0f; }
        ImGui::SameLine();
        if(ImGui::Button("16##grid", ImVec2(40, 0))) { this->grid_size = 16.0f; }
        
        ImGui::SameLine();
        ImGui::Text("|");
        ImGui::SameLine();
        if(ImGui::Button("Clear Walls")) {
            this->clear_walls();
        }
        ImGui::SameLine();
        if(ImGui::Button("Detect Rooms")) {
            this->detect_rooms();
        }

        ImGui::SameLine();
        ImGui::Text("|");
        ImGui::SameLine();
        ImGui::Text("Map Path:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        ImGui::InputText("##map_path", this->map_path_buffer, sizeof(this->map_path_buffer));
        ImGui::SameLine();
        if(ImGui::Button("Save Map")) {
            EmberEditor::save_map(this->map_path_buffer, this->walls, this->rooms);
        }

        ImGui::Separator();

        ImVec2 available_size = ImGui::GetContentRegionAvail();
        
        // Handle resize
        if(available_size.x != this->viewport_size.x || available_size.y != this->viewport_size.y) {
            if(available_size.x > 0 && available_size.y > 0) {
                ::UnloadRenderTexture(this->render_texture);
                this->viewport_size = available_size;
                this->render_texture = ::LoadRenderTexture(static_cast<int>(this->viewport_size.x), 
                                                           static_cast<int>(this->viewport_size.y));
                
                // Update camera offset for new viewport size
                this->camera.set_offset({ this->viewport_size.x * 0.5f, this->viewport_size.y * 0.5f });
            }
        }

        // Display the rendered texture
        ImVec2 texture_size(static_cast<float>(this->render_texture.texture.width),
                           static_cast<float>(this->render_texture.texture.height));
        
        rlImGuiImageRenderTexture(&this->render_texture);

        // Track hover and focus state
        this->is_hovered_ = ImGui::IsItemHovered();
        this->is_focused_ = ImGui::IsWindowFocused();

        // Handle input
        this->handle_input();
    }
    ImGui::End();
}

EmberEngine::Camera2D& EditorViewport2D::get_camera()
{
    return this->camera;
}

const EmberEngine::Camera2D& EditorViewport2D::get_camera() const
{
    return this->camera;
}

bool EditorViewport2D::is_hovered() const
{
    return this->is_hovered_;
}

bool EditorViewport2D::is_focused() const
{
    return this->is_focused_;
}

::Vector2 EditorViewport2D::get_viewport_size() const
{
    return { this->viewport_size.x, this->viewport_size.y };
}

::Vector2 EditorViewport2D::get_viewport_position() const
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    return { pos.x, pos.y };
}

void EditorViewport2D::set_grid_size(float size)
{
    this->grid_size = size;
}

float EditorViewport2D::get_grid_size() const
{
    return this->grid_size;
}

void EditorViewport2D::render_grid()
{
    float grid_offset_x = std::fmod(this->camera.get_target().x, this->grid_size);
    float grid_offset_y = std::fmod(this->camera.get_target().y, this->grid_size);

    // Calculate visible bounds
    float half_width = this->viewport_size.x / (2.0f * this->camera.get_zoom());
    float half_height = this->viewport_size.y / (2.0f * this->camera.get_zoom());

    float left = this->camera.get_target().x - half_width;
    float right = this->camera.get_target().x + half_width;
    float top = this->camera.get_target().y - half_height;
    float bottom = this->camera.get_target().y + half_height;

    // Draw vertical grid lines
    float x = std::floor(left / this->grid_size) * this->grid_size;
    while(x <= right) {
        ::DrawLine(static_cast<int>(x), static_cast<int>(top), static_cast<int>(x), static_cast<int>(bottom), 
                   ::Color{ 64, 64, 64, 128 });
        x += this->grid_size;
    }

    // Draw horizontal grid lines
    float y = std::floor(top / this->grid_size) * this->grid_size;
    while(y <= bottom) {
        ::DrawLine(static_cast<int>(left), static_cast<int>(y), static_cast<int>(right), static_cast<int>(y), 
                   ::Color{ 64, 64, 64, 128 });
        y += this->grid_size;
    }
}

void EditorViewport2D::render_walls()
{
    for(size_t i = 0; i < this->walls.size(); ++i) {
        const auto& wall = this->walls[i];
        bool is_selected = (static_cast<int>(i) == this->selected_wall_index);
        bool is_dragging_this = (this->is_dragging_vertex && static_cast<int>(i) == this->dragging_wall_index);

        ::Color line_color = wall.is_portal ? SKYBLUE : WHITE;
        ::Color point_color = wall.is_portal ? SKYBLUE : BLUE;
        if(is_selected) {
            line_color = YELLOW;
            point_color = GREEN;
        }
        
        // Highlight dragging vertex
        if(is_dragging_this) {
            ::Color drag_color = ORANGE;
            if(this->dragging_start_vertex) {
                ::DrawCircleV(wall.start, 5.0f, drag_color);
            } else {
                ::DrawCircleV(wall.end, 5.0f, drag_color);
            }
        }
        
        ::DrawLineEx(wall.start, wall.end, 3.0f, line_color);
        ::DrawCircleV(wall.start, 3.0f, point_color);
        ::DrawCircleV(wall.end, 3.0f, point_color);
    }
}

void EditorViewport2D::handle_input()
{
    if(!this->is_hovered_) {
        return;
    }

    // Get mouse position in world space
    ImVec2 mouse_screen = ImGui::GetMousePos();
    this->current_mouse_world_pos = this->screen_to_world(mouse_screen);
    
    // Snap to grid if needed
    if(this->current_mode == EditorMode::PlaceWall) {
        this->current_mouse_world_pos = this->snap_to_grid(this->current_mouse_world_pos);
    }

    // Camera controls (always active with middle mouse)
    if(ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
        ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle);
        ::Vector2 current_target = this->camera.get_target();
        this->camera.set_target({ current_target.x - delta.x / this->camera.get_zoom(), 
                                 current_target.y - delta.y / this->camera.get_zoom() });
        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
    }

    // Zoom with mouse wheel
    ImGuiIO& io = ImGui::GetIO();
    if(io.MouseWheel != 0.0f) {
        float zoom = this->camera.get_zoom();
        zoom += io.MouseWheel * 0.1f;
        zoom = zoom < 0.1f ? 0.1f : zoom;
        this->camera.set_zoom(zoom);
    }

    // Mode-specific input
    if(this->current_mode == EditorMode::PlaceWall) {
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if(!this->is_placing_wall) {
                // Start placing wall
                this->is_placing_wall = true;
                this->wall_start_pos = this->current_mouse_world_pos;
            } else {
                // Finish placing wall
                this->add_wall(Wall(this->wall_start_pos, this->current_mouse_world_pos));
                this->is_placing_wall = false;
            }
        }
        
        // Cancel with right click or Escape
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            this->is_placing_wall = false;
        }
    }
    else if(this->current_mode == EditorMode::Select) {
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            // First check if clicking on a vertex
            float vertex_threshold = 8.0f / this->camera.get_zoom();
            int closest_wall = -1;
            bool is_start = true;
            float closest_vertex_distance = vertex_threshold;
            
            for(size_t i = 0; i < this->walls.size(); ++i) {
                float dist_start = this->distance_to_point(this->current_mouse_world_pos, this->walls[i].start);
                float dist_end = this->distance_to_point(this->current_mouse_world_pos, this->walls[i].end);
                
                if(dist_start < closest_vertex_distance) {
                    closest_vertex_distance = dist_start;
                    closest_wall = static_cast<int>(i);
                    is_start = true;
                }
                if(dist_end < closest_vertex_distance) {
                    closest_vertex_distance = dist_end;
                    closest_wall = static_cast<int>(i);
                    is_start = false;
                }
            }
            
            if(closest_wall != -1) {
                // Start dragging vertex
                this->is_dragging_vertex = true;
                this->dragging_wall_index = closest_wall;
                this->dragging_start_vertex = is_start;
                this->selected_wall_index = closest_wall;
            } else {
                // Check if clicking on a wall line
                float selection_threshold = 10.0f / this->camera.get_zoom();
                closest_wall = -1;
                float closest_distance = selection_threshold;
                
                for(size_t i = 0; i < this->walls.size(); ++i) {
                    float dist = this->distance_to_line(this->current_mouse_world_pos, 
                                                        this->walls[i].start, 
                                                        this->walls[i].end);
                    if(dist < closest_distance) {
                        closest_distance = dist;
                        closest_wall = static_cast<int>(i);
                    }
                }
                
                this->selected_wall_index = closest_wall;
            }
            
            // Check if clicking inside a room (only if not dragging vertex)
            if(closest_wall == -1 && !this->is_dragging_vertex) {
                for(size_t i = 0; i < this->rooms.size(); ++i) {
                    if(this->point_in_polygon(this->current_mouse_world_pos, this->rooms[i].vertices)) {
                        this->selected_room_index = static_cast<int>(i);
                        break;
                    }
                }
            }
        }
        
        // Handle vertex dragging
        if(this->is_dragging_vertex && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            ::Vector2 snapped_pos = this->snap_to_grid(this->current_mouse_world_pos);
            if(this->dragging_start_vertex) {
                this->walls[this->dragging_wall_index].start = snapped_pos;
            } else {
                this->walls[this->dragging_wall_index].end = snapped_pos;
            }
        }
        
        // Stop dragging when mouse released
        if(this->is_dragging_vertex && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            this->is_dragging_vertex = false;
            this->dragging_wall_index = -1;
        }
        
        // Cancel with right click or Escape
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            this->is_placing_wall = false;
            this->is_dragging_vertex = false;
        }
    }
}

::Vector2 EditorViewport2D::screen_to_world(ImVec2 screen_pos)
{
    // Get viewport start position
    ImVec2 viewport_pos = ImGui::GetItemRectMin();
    
    // Convert screen to viewport local coordinates
    float local_x = screen_pos.x - viewport_pos.x;
    float local_y = screen_pos.y - viewport_pos.y;
    
    // Convert to world coordinates
    ::Vector2 offset = this->camera.get_offset();
    ::Vector2 target = this->camera.get_target();
    float zoom = this->camera.get_zoom();
    
    float world_x = target.x + (local_x - offset.x) / zoom;
    float world_y = target.y + (local_y - offset.y) / zoom;
    
    return { world_x, world_y };
}

::Vector2 EditorViewport2D::snap_to_grid(::Vector2 pos)
{
    float snapped_x = std::round(pos.x / this->grid_size) * this->grid_size;
    float snapped_y = std::round(pos.y / this->grid_size) * this->grid_size;
    return { snapped_x, snapped_y };
}

void EditorViewport2D::add_wall(const Wall& wall)
{
    this->walls.push_back(wall);
}

void EditorViewport2D::clear_walls()
{
    this->walls.clear();
}

const std::vector<Wall>& EditorViewport2D::get_walls() const
{
    return this->walls;
}

std::vector<Wall>& EditorViewport2D::get_walls_mutable()
{
    return this->walls;
}

int EditorViewport2D::get_selected_wall_index() const
{
    return this->selected_wall_index;
}

void EditorViewport2D::set_selected_wall_index(int index)
{
    this->selected_wall_index = index;
}

bool EditorViewport2D::has_selection() const
{
    return this->selected_wall_index >= 0 && this->selected_wall_index < static_cast<int>(this->walls.size());
}

float EditorViewport2D::distance_to_line(::Vector2 point, ::Vector2 line_start, ::Vector2 line_end)
{
    float dx = line_end.x - line_start.x;
    float dy = line_end.y - line_start.y;
    float length_sq = dx * dx + dy * dy;
    
    if(length_sq == 0.0f) {
        // Line is actually a point
        float px = point.x - line_start.x;
        float py = point.y - line_start.y;
        return std::sqrt(px * px + py * py);
    }
    
    // Calculate projection parameter
    float t = ((point.x - line_start.x) * dx + (point.y - line_start.y) * dy) / length_sq;
    t = std::max(0.0f, std::min(1.0f, t));
    
    // Find closest point on line segment
    float closest_x = line_start.x + t * dx;
    float closest_y = line_start.y + t * dy;
    
    // Calculate distance
    float dist_x = point.x - closest_x;
    float dist_y = point.y - closest_y;
    return std::sqrt(dist_x * dist_x + dist_y * dist_y);
}

float EditorViewport2D::distance_to_point(::Vector2 p1, ::Vector2 p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool EditorViewport2D::point_in_polygon(::Vector2 point, const std::vector<::Vector2>& polygon)
{
    if(polygon.size() < 3) return false;
    
    int intersections = 0;
    for(size_t i = 0; i < polygon.size(); ++i) {
        ::Vector2 v1 = polygon[i];
        ::Vector2 v2 = polygon[(i + 1) % polygon.size()];
        
        if((v1.y > point.y) != (v2.y > point.y)) {
            float x_intersection = (v2.x - v1.x) * (point.y - v1.y) / (v2.y - v1.y) + v1.x;
            if(point.x < x_intersection) {
                intersections++;
            }
        }
    }
    
    return (intersections % 2) == 1;
}

void EditorViewport2D::render_rooms()
{
    for(size_t i = 0; i < this->rooms.size(); ++i) {
        const auto& room = this->rooms[i];
        bool is_selected = (static_cast<int>(i) == this->selected_room_index);
        
        if(room.vertices.size() >= 3) {
            // Draw semi-transparent fill
            ::Color fill_color = room.color;
            if(is_selected) {
                fill_color.a = static_cast<unsigned char>(std::min(200, static_cast<int>(fill_color.a) + 80));
            }
            
            // Draw triangulated polygon
            for(size_t j = 1; j < room.vertices.size() - 1; ++j) {
                ::DrawTriangle(room.vertices[0], room.vertices[j], room.vertices[j + 1], fill_color);
            }
        }
    }
}

void EditorViewport2D::detect_rooms()
{
    this->rooms.clear();
    
    if(this->walls.size() < 3) return;

    for(auto& wall : this->walls) {
        wall.portal_room_a = -1;
        wall.portal_room_b = -1;
    }
    
    // Track which walls have been used
    std::vector<bool> used(this->walls.size(), false);
    
    // Try to find closed loops starting from each unused wall
    for(size_t start_idx = 0; start_idx < this->walls.size(); ++start_idx) {
        if(this->walls[start_idx].is_portal || used[start_idx]) {
            continue;
        }
        
        std::vector<::Vector2> vertices;
        std::vector<size_t> wall_indices;
        
        // Start tracing from this wall
        ::Vector2 current_pos = this->walls[start_idx].start;
        ::Vector2 target_pos = this->walls[start_idx].end;
        vertices.push_back(current_pos);
        wall_indices.push_back(start_idx);
        
        current_pos = target_pos;
        
        // Try to find a closed loop
        bool found_loop = false;
        const float snap_threshold = 1.0f;
        
        for(int steps = 0; steps < static_cast<int>(this->walls.size()); ++steps) {
            // Check if we closed the loop
            float dist_to_start = this->distance_to_point(current_pos, vertices[0]);
            if(wall_indices.size() >= 3 && dist_to_start < snap_threshold) {
                found_loop = true;
                break;
            }
            
            // Find next connected wall
            bool found_next = false;
            for(size_t i = 0; i < this->walls.size(); ++i) {
                if(std::find(wall_indices.begin(), wall_indices.end(), i) != wall_indices.end()) {
                    continue; // Already used in this loop
                }

                if(used[i] && !this->walls[i].is_portal) {
                    continue;
                }
                
                const auto& wall = this->walls[i];
                
                // Check if this wall starts where we are
                if(this->distance_to_point(current_pos, wall.start) < snap_threshold) {
                    vertices.push_back(wall.start);
                    wall_indices.push_back(i);
                    current_pos = wall.end;
                    found_next = true;
                    break;
                }
                // Check if this wall ends where we are (traverse in reverse)
                else if(this->distance_to_point(current_pos, wall.end) < snap_threshold) {
                    vertices.push_back(wall.end);
                    wall_indices.push_back(i);
                    current_pos = wall.start;
                    found_next = true;
                    break;
                }
            }
            
            if(!found_next) break;
        }
        
        // If we found a closed loop, create a room
        if(found_loop && vertices.size() >= 3) {
            Room room;
            room.vertices = vertices;
            room.color = {
                static_cast<unsigned char>(::GetRandomValue(60, 200)),
                static_cast<unsigned char>(::GetRandomValue(60, 200)),
                static_cast<unsigned char>(::GetRandomValue(60, 200)),
                80
            };
            int room_index = static_cast<int>(this->rooms.size());
            this->rooms.push_back(room);
            
            // Mark these walls as used
            for(size_t idx : wall_indices) {
                if(!this->walls[idx].is_portal) {
                    used[idx] = true;
                } else {
                    if(this->walls[idx].portal_room_a == -1) {
                        this->walls[idx].portal_room_a = room_index;
                    } else if(this->walls[idx].portal_room_b == -1
                        && this->walls[idx].portal_room_a != room_index) {
                        this->walls[idx].portal_room_b = room_index;
                    }
                }
            }
        }
    }
}

const std::vector<Room>& EditorViewport2D::get_rooms() const
{
    return this->rooms;
}

std::vector<Room>& EditorViewport2D::get_rooms_mutable()
{
    return this->rooms;
}

int EditorViewport2D::get_selected_room_index() const
{
    return this->selected_room_index;
}

void EditorViewport2D::set_selected_room_index(int index)
{
    this->selected_room_index = index;
}

bool EditorViewport2D::has_room_selection() const
{
    return this->selected_room_index >= 0 && this->selected_room_index < static_cast<int>(this->rooms.size());
}
