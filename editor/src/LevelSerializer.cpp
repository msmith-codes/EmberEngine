#include "LevelSerializer.hpp"

#include "panels/EditorViewport2D.hpp"

#include <EmberEngine/core/Logger.hpp>

#include <fstream>
#include <iomanip>
#include <sstream>

namespace EmberEditor
{
    static std::string json_escape(const std::string& input)
    {
        std::ostringstream out;
        for(char c : input) {
            switch(c) {
                case '\\': out << "\\\\"; break;
                case '"': out << "\\\""; break;
                case '\n': out << "\\n"; break;
                case '\r': out << "\\r"; break;
                case '\t': out << "\\t"; break;
                default:
                    if(static_cast<unsigned char>(c) < 0x20) {
                        out << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                            << static_cast<int>(static_cast<unsigned char>(c))
                            << std::dec << std::setfill(' ');
                    } else {
                        out << c;
                    }
                    break;
            }
        }
        return out.str();
    }

    bool save_map(const std::string& path, const std::vector<Wall>& walls, const std::vector<Room>& rooms)
    {
        std::ofstream file(path, std::ios::out | std::ios::trunc);
        if(!file.is_open()) {
            EmberEngine::Logger::error("LevelSerializer", "Failed to open map file: " + path);
            return false;
        }

        file << std::fixed << std::setprecision(3);
        file << "{\n";
        file << "  \"version\": 1,\n";
        file << "  \"walls\": [\n";
        for(size_t i = 0; i < walls.size(); ++i) {
            const Wall& wall = walls[i];
            file << "    {\n";
            file << "      \"start\": {\"x\": " << wall.start.x << ", \"y\": " << wall.start.y << "},\n";
            file << "      \"end\": {\"x\": " << wall.end.x << ", \"y\": " << wall.end.y << "},\n";
            file << "      \"height\": " << wall.height << ",\n";
            file << "      \"texture\": \"" << json_escape(wall.texture_path) << "\",\n";
            file << "      \"portal\": " << (wall.is_portal ? "true" : "false") << "\n";
            file << "    }";
            if(i + 1 < walls.size()) {
                file << ",";
            }
            file << "\n";
        }
        file << "  ],\n";
        file << "  \"rooms\": [\n";
        for(size_t i = 0; i < rooms.size(); ++i) {
            const Room& room = rooms[i];
            file << "    {\n";
            file << "      \"vertices\": [";
            for(size_t v = 0; v < room.vertices.size(); ++v) {
                const ::Vector2& vertex = room.vertices[v];
                file << "{\"x\": " << vertex.x << ", \"y\": " << vertex.y << "}";
                if(v + 1 < room.vertices.size()) {
                    file << ", ";
                }
            }
            file << "],\n";
            file << "      \"floor_height\": " << room.floor_height << ",\n";
            file << "      \"ceiling_height\": " << room.ceiling_height << ",\n";
            file << "      \"floor_texture\": \"" << json_escape(room.floor_texture_path) << "\",\n";
            file << "      \"ceiling_texture\": \"" << json_escape(room.ceiling_texture_path) << "\"\n";
            file << "    }";
            if(i + 1 < rooms.size()) {
                file << ",";
            }
            file << "\n";
        }
        file << "  ]\n";
        file << "}\n";

        if(!file.good()) {
            EmberEngine::Logger::error("LevelSerializer", "Failed writing map file: " + path);
            return false;
        }

        EmberEngine::Logger::info("LevelSerializer", "Saved map to " + path);
        return true;
    }
}
