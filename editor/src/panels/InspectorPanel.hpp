#pragma once

#include <imgui.h>
#include <raylib.h>
#include <vector>

// Forward declare Wall and Room
struct Wall;
struct Room;

class InspectorPanel
{
    public:
        InspectorPanel();
        ~InspectorPanel();

        void render_ui(std::vector<Wall>* walls, int selected_wall_index,
                      std::vector<Room>* rooms, int selected_room_index);
        
    private:
        void render_wall_properties(Wall& wall);
        void render_room_properties(Room& room);
};