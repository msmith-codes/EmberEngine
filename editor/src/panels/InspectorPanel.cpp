#include "InspectorPanel.hpp"
#include "EditorViewport2D.hpp"
#include <cmath>

InspectorPanel::InspectorPanel()
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::render_ui(std::vector<Wall>* walls, int selected_wall_index,
                              std::vector<Room>* rooms, int selected_room_index)
{
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    
    if(ImGui::Begin("Inspector")) {
        bool has_wall_selection = walls && selected_wall_index >= 0 && selected_wall_index < static_cast<int>(walls->size());
        bool has_room_selection = rooms && selected_room_index >= 0 && selected_room_index < static_cast<int>(rooms->size());
        
        if(has_room_selection) {
            Room& room = (*rooms)[selected_room_index];
            this->render_room_properties(room);
        } else if(has_wall_selection) {
            Wall& wall = (*walls)[selected_wall_index];
            this->render_wall_properties(wall);
        } else {
            ImGui::TextDisabled("No selection");
            ImGui::Separator();
            ImGui::TextWrapped("Select a wall or click inside a room to view and edit properties.");
        }
    }
    ImGui::End();
}

void InspectorPanel::render_wall_properties(Wall& wall)
{
    ImGui::Text("Wall Properties");
    ImGui::Separator();
    
    // Calculate wall length
    float dx = wall.end.x - wall.start.x;
    float dy = wall.end.y - wall.start.y;
    float length = std::sqrt(dx * dx + dy * dy);
    
    // Start position
    ImGui::Text("Start Position");
    ImGui::DragFloat("X##start", &wall.start.x, 1.0f, -10000.0f, 10000.0f, "%.1f");
    ImGui::DragFloat("Y##start", &wall.start.y, 1.0f, -10000.0f, 10000.0f, "%.1f");
    
    ImGui::Separator();
    
    // End position
    ImGui::Text("End Position");
    ImGui::DragFloat("X##end", &wall.end.x, 1.0f, -10000.0f, 10000.0f, "%.1f");
    ImGui::DragFloat("Y##end", &wall.end.y, 1.0f, -10000.0f, 10000.0f, "%.1f");
    
    ImGui::Separator();
    
    // Wall height
    ImGui::Text("Wall Height");
    ImGui::DragFloat("##height", &wall.height, 1.0f, 1.0f, 1000.0f, "%.1f");
    
    ImGui::Separator();
    
    // Texture path
    ImGui::Text("Wall Texture");
    char texture_buffer[256];
    strncpy(texture_buffer, wall.texture_path.c_str(), 255);
    texture_buffer[255] = '\0';
    if(ImGui::InputText("##wall_texture", texture_buffer, 256)) {
        wall.texture_path = texture_buffer;
    }
    if(ImGui::Button("Clear Texture##wall")) {
        wall.texture_path = "";
    }

    ImGui::Separator();

    ImGui::Text("Portal Settings");
    ImGui::Checkbox("Is Portal", &wall.is_portal);
    if(wall.is_portal) {
        if(wall.portal_room_a != -1 || wall.portal_room_b != -1) {
            ImGui::Text("Connects Rooms: %d, %d", wall.portal_room_a, wall.portal_room_b);
        } else {
            ImGui::TextDisabled("Connects Rooms: (detect rooms)");
        }
    }
    
    ImGui::Separator();
    
    // Display calculated properties (read-only)
    ImGui::TextDisabled("Calculated Properties");
    ImGui::Text("Length: %.2f units", length);
    
    // Calculate angle
    float angle_rad = std::atan2(dy, dx);
    float angle_deg = angle_rad * 180.0f / 3.14159265f;
    ImGui::Text("Angle: %.1f degrees", angle_deg);
}

void InspectorPanel::render_room_properties(Room& room)
{
    ImGui::Text("Room Properties");
    ImGui::Separator();
    
    // Floor height
    ImGui::Text("Floor Height");
    ImGui::DragFloat("##floor", &room.floor_height, 1.0f, -1000.0f, 1000.0f, "%.1f");
    
    ImGui::Separator();
    
    // Ceiling height
    ImGui::Text("Ceiling Height");
    ImGui::DragFloat("##ceiling", &room.ceiling_height, 1.0f, -1000.0f, 1000.0f, "%.1f");
    
    ImGui::Separator();
    
    // Floor texture
    ImGui::Text("Floor Texture");
    char floor_buffer[256];
    strncpy(floor_buffer, room.floor_texture_path.c_str(), 255);
    floor_buffer[255] = '\0';
    if(ImGui::InputText("##floor_texture", floor_buffer, 256)) {
        room.floor_texture_path = floor_buffer;
    }
    if(ImGui::Button("Clear Texture##floor")) {
        room.floor_texture_path = "";
    }
    
    ImGui::Separator();
    
    // Ceiling texture
    ImGui::Text("Ceiling Texture");
    char ceiling_buffer[256];
    strncpy(ceiling_buffer, room.ceiling_texture_path.c_str(), 255);
    ceiling_buffer[255] = '\0';
    if(ImGui::InputText("##ceiling_texture", ceiling_buffer, 256)) {
        room.ceiling_texture_path = ceiling_buffer;
    }
    if(ImGui::Button("Clear Texture##ceiling")) {
        room.ceiling_texture_path = "";
    }

    ImGui::Separator();

    // Room color
    ImGui::Text("Room Color");
    float color[4] = {
        room.color.r / 255.0f,
        room.color.g / 255.0f,
        room.color.b / 255.0f,
        room.color.a / 255.0f
    };
    if(ImGui::ColorEdit4("##room_color", color)) {
        room.color.r = static_cast<unsigned char>(color[0] * 255.0f);
        room.color.g = static_cast<unsigned char>(color[1] * 255.0f);
        room.color.b = static_cast<unsigned char>(color[2] * 255.0f);
        room.color.a = static_cast<unsigned char>(color[3] * 255.0f);
    }
    
    ImGui::Separator();
    
    // Display calculated properties (read-only)
    ImGui::TextDisabled("Calculated Properties");
    ImGui::Text("Vertices: %zu", room.vertices.size());
    
    // Calculate room height
    float room_height = room.ceiling_height - room.floor_height;
    ImGui::Text("Room Height: %.2f units", room_height);
}
