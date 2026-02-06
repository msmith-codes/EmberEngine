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

    bool load_map(const std::string& path, std::vector<Wall>& walls, std::vector<Room>& rooms)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            EmberEngine::Logger::error("LevelSerializer", "Failed to open map file: " + path);
            return false;
        }

        // Read entire file into string
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Clear existing data
        walls.clear();
        rooms.clear();

        // Helper function to extract number after a pattern
        auto extract_number = [](const std::string& text, const std::string& pattern, size_t start_pos = 0) -> float {
            size_t pos = text.find(pattern, start_pos);
            if (pos == std::string::npos) return 0.0f;
            
            pos += pattern.length();
            // Skip whitespace
            while (pos < text.length() && (text[pos] == ' ' || text[pos] == '\t')) pos++;
            
            // Find end of number
            size_t end_pos = pos;
            while (end_pos < text.length() && 
                   (std::isdigit(text[end_pos]) || text[end_pos] == '.' || 
                    text[end_pos] == '-' || text[end_pos] == '+' || text[end_pos] == 'e' || text[end_pos] == 'E')) {
                end_pos++;
            }
            
            if (end_pos == pos) return 0.0f;
            
            try {
                return std::stof(text.substr(pos, end_pos - pos));
            } catch (...) {
                return 0.0f;
            }
        };

        // Helper function to extract string value
        auto extract_string = [](const std::string& text, const std::string& pattern, size_t start_pos = 0) -> std::string {
            size_t pos = text.find(pattern, start_pos);
            if (pos == std::string::npos) return "";
            
            pos += pattern.length();
            // Skip whitespace and find opening quote
            while (pos < text.length() && text[pos] != '"') pos++;
            if (pos >= text.length()) return "";
            
            pos++; // Skip opening quote
            size_t end_pos = pos;
            while (end_pos < text.length() && text[end_pos] != '"') end_pos++;
            
            if (end_pos == pos) return "";
            return text.substr(pos, end_pos - pos);
        };

        // Parse walls
        size_t walls_start = content.find("\"walls\": [");
        if (walls_start == std::string::npos) {
            EmberEngine::Logger::error("LevelSerializer", "Invalid map file format: no walls array");
            return false;
        }

        size_t walls_begin = content.find("[", walls_start);
        size_t walls_end = content.find("]", walls_begin);
        
        if (walls_begin == std::string::npos || walls_end == std::string::npos) {
            EmberEngine::Logger::error("LevelSerializer", "Invalid walls array format");
            return false;
        }

        size_t pos = walls_begin + 1; // Start after the opening bracket
        int wall_count = 0;
        
        while (pos < walls_end) {
            // Find next wall object
            size_t wall_start = content.find("{", pos);
            if (wall_start == std::string::npos || wall_start >= walls_end) {
                break;
            }

            // Find the matching closing brace for this wall
            size_t brace_count = 1;
            size_t wall_end = wall_start + 1;
            while (wall_end < walls_end && brace_count > 0) {
                if (content[wall_end] == '{') brace_count++;
                else if (content[wall_end] == '}') brace_count--;
                wall_end++;
            }
            
            if (brace_count != 0) {
                EmberEngine::Logger::error("LevelSerializer", "Malformed wall object");
                break;
            }
            wall_end--; // Point to the closing brace

            std::string wall_data = content.substr(wall_start, wall_end - wall_start + 1);
            
            // Create wall with default values
            Wall wall(::Vector2{0, 0}, ::Vector2{0, 0});
            
            // Parse start position - look for the complete "start": {...} block
            size_t start_section = wall_data.find("\"start\":");
            if (start_section != std::string::npos) {
                size_t start_brace = wall_data.find("{", start_section);
                size_t start_end = wall_data.find("}", start_brace);
                if (start_brace != std::string::npos && start_end != std::string::npos) {
                    std::string start_block = wall_data.substr(start_brace, start_end - start_brace + 1);
                    wall.start.x = extract_number(start_block, "\"x\":");
                    wall.start.y = extract_number(start_block, "\"y\":");
                }
            }
            
            // Parse end position - look for the complete "end": {...} block  
            size_t end_section = wall_data.find("\"end\":");
            if (end_section != std::string::npos) {
                size_t end_brace = wall_data.find("{", end_section);
                size_t end_end = wall_data.find("}", end_brace);
                if (end_brace != std::string::npos && end_end != std::string::npos) {
                    std::string end_block = wall_data.substr(end_brace, end_end - end_brace + 1);
                    wall.end.x = extract_number(end_block, "\"x\":");
                    wall.end.y = extract_number(end_block, "\"y\":");
                }
            }
            
            // Parse height
            wall.height = extract_number(wall_data, "\"height\":");
            if (wall.height == 0.0f) wall.height = 64.0f; // Default height
            
            // Parse texture
            wall.texture_path = extract_string(wall_data, "\"texture\":");
            
            // Parse portal
            size_t portal_pos = wall_data.find("\"portal\":");
            if (portal_pos != std::string::npos) {
                wall.is_portal = wall_data.find("true", portal_pos) != std::string::npos;
            }
            
            walls.push_back(wall);
            wall_count++;
            
            // Debug logging
            EmberEngine::Logger::info("LevelSerializer", 
                "Wall " + std::to_string(wall_count) + ": (" + std::to_string(wall.start.x) + "," + std::to_string(wall.start.y) + 
                ") to (" + std::to_string(wall.end.x) + "," + std::to_string(wall.end.y) + 
                ") height=" + std::to_string(wall.height));
                
            pos = wall_end + 1;
        }

        // Parse rooms (simplified - just create basic rooms for now)
        size_t rooms_start = content.find("\"rooms\": [");
        if (rooms_start != std::string::npos) {
            size_t rooms_begin = content.find("[", rooms_start);
            size_t rooms_end = content.find("]", rooms_begin);
            
            if (rooms_begin != std::string::npos && rooms_end != std::string::npos) {
                size_t pos = rooms_begin + 1;
                int room_count = 0;
                
                while (pos < rooms_end) {
                    size_t room_start = content.find("{", pos);
                    if (room_start == std::string::npos || room_start >= rooms_end) {
                        break;
                    }

                    // Find matching closing brace
                    size_t brace_count = 1;
                    size_t room_end = room_start + 1;
                    while (room_end < rooms_end && brace_count > 0) {
                        if (content[room_end] == '{') brace_count++;
                        else if (content[room_end] == '}') brace_count--;
                        room_end++;
                    }
                    
                    if (brace_count != 0) break;
                    room_end--;

                    std::string room_data = content.substr(room_start, room_end - room_start + 1);
                    
                    Room room;
                    
                    // Parse vertices array
                    size_t vertices_start = room_data.find("\"vertices\": [");
                    if (vertices_start != std::string::npos) {
                        size_t vertices_begin = room_data.find("[", vertices_start);
                        size_t vertices_end = room_data.find("]", vertices_begin);
                        
                        if (vertices_begin != std::string::npos && vertices_end != std::string::npos) {
                            std::string vertices_data = room_data.substr(vertices_begin, vertices_end - vertices_begin + 1);
                            
                            // Parse each vertex
                            size_t vertex_pos = 0;
                            while (true) {
                                size_t vertex_start = vertices_data.find("{", vertex_pos);
                                if (vertex_start == std::string::npos) break;
                                
                                size_t vertex_end = vertices_data.find("}", vertex_start);
                                if (vertex_end == std::string::npos) break;
                                
                                std::string vertex_data = vertices_data.substr(vertex_start, vertex_end - vertex_start + 1);
                                
                                ::Vector2 vertex;
                                vertex.x = extract_number(vertex_data, "\"x\":");
                                vertex.y = extract_number(vertex_data, "\"y\":");
                                room.vertices.push_back(vertex);
                                
                                vertex_pos = vertex_end + 1;
                            }
                        }
                    }
                    
                    // Assign random color to each room
                    room.color = {
                        static_cast<unsigned char>(::GetRandomValue(60, 200)),
                        static_cast<unsigned char>(::GetRandomValue(60, 200)),
                        static_cast<unsigned char>(::GetRandomValue(60, 200)),
                        80
                    };
                    
                    room_count++;
                    rooms.push_back(room);
                    pos = room_end + 1;
                    
                    // Debug logging
                    EmberEngine::Logger::info("LevelSerializer", 
                        "Room " + std::to_string(room_count) + ": " + std::to_string(room.vertices.size()) + " vertices");
                }
            }
        }

        EmberEngine::Logger::info("LevelSerializer", "Loaded map from " + path + " (" + std::to_string(walls.size()) + " walls, " + std::to_string(rooms.size()) + " rooms)");
        return true;
    }
}
