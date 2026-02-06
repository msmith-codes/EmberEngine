#pragma once

#include <string>
#include <vector>

struct Wall;
struct Room;

namespace EmberEditor
{
    bool save_map(const std::string& path, const std::vector<Wall>& walls, const std::vector<Room>& rooms);
    bool load_map(const std::string& path, std::vector<Wall>& walls, std::vector<Room>& rooms);
}
