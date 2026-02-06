#include <panels/FileTree.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

FileTree::FileTree() 
    : selected_file(nullptr) {
    // Set default root to project root directory (current working directory)
    root_directory = ".";
}

FileTree::~FileTree() {
}

void FileTree::render_ui() {
    ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("File Explorer")) {
        // Root path display
        ImGui::Text("Project Root (res://)");
        ImGui::Separator();
        
        // Refresh button
        if (ImGui::Button("Refresh")) {
            refresh();
        }
        ImGui::SameLine();
        
        // Show current selection
        if (selected_file) {
            ImGui::Text("Selected: %s", selected_file->name.c_str());
        } else {
            ImGui::TextDisabled("No selection");
        }
        
        ImGui::Separator();
        
        // File tree view
        if (ImGui::BeginChild("FileTreeView", ImVec2(0, -30))) {
            render_directory_tree(root_directory);
        }
        ImGui::EndChild();
        
        // Status bar
        ImGui::Separator();
        if (selected_file) {
            if (selected_file->is_directory) {
                ImGui::Text("Directory: %s", selected_file->res_path.c_str());
            } else {
                ImGui::Text("%s (%.2f KB)", selected_file->res_path.c_str(), 
                           selected_file->file_size / 1024.0f);
            }
        }
    }
    ImGui::End();
}

void FileTree::set_root_directory(const std::string& path) {
    root_directory = path;
    selected_file = nullptr;
    refresh();
}

const FileEntry* FileTree::get_selected_file() const {
    return selected_file;
}

std::string FileTree::to_res_path(const std::string& filesystem_path) const {
    // Convert filesystem path to res:// protocol
    std::filesystem::path fs_path(filesystem_path);
    std::filesystem::path root_path(root_directory);
    
    try {
        auto relative = std::filesystem::relative(fs_path, root_path);
        std::string res_path = "res://" + relative.string();
        // Normalize path separators to forward slashes
        std::replace(res_path.begin(), res_path.end(), '\\', '/');
        return res_path;
    } catch (const std::exception&) {
        return "res://" + filesystem_path;
    }
}

std::string FileTree::from_res_path(const std::string& res_path) const {
    if (res_path.length() >= 6 && res_path.substr(0, 6) == "res://") {
        std::string relative_path = res_path.substr(6); // Remove "res://"
        return root_directory + "/" + relative_path;
    }
    return res_path;
}

void FileTree::set_file_activated_callback(std::function<void(const FileEntry&)> callback) {
    file_activated_callback = callback;
}

void FileTree::refresh() {
    current_entries.clear();
    scan_directory(root_directory);
}

void FileTree::scan_directory(const std::string& directory_path) {
    current_entries.clear();
    
    try {
        if (!std::filesystem::exists(directory_path)) {
            return;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            FileEntry file_entry;
            file_entry.name = entry.path().filename().string();
            file_entry.path = entry.path().string();
            file_entry.res_path = to_res_path(file_entry.path);
            file_entry.is_directory = entry.is_directory();
            
            if (!file_entry.is_directory) {
                file_entry.extension = get_file_extension(file_entry.name);
                file_entry.file_size = std::filesystem::file_size(entry.path());
            } else {
                file_entry.file_size = 0;
            }
            
            current_entries.push_back(file_entry);
        }
        
        // Sort entries: directories first, then files alphabetically
        std::sort(current_entries.begin(), current_entries.end(), 
                  [](const FileEntry& a, const FileEntry& b) {
                      if (a.is_directory != b.is_directory) {
                          return a.is_directory > b.is_directory;
                      }
                      return a.name < b.name;
                  });
                  
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}

void FileTree::render_directory_tree(const std::string& directory_path, int depth) {
    try {
        if (!std::filesystem::exists(directory_path)) {
            return;
        }
        
        std::vector<FileEntry> entries;
        
        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            FileEntry file_entry;
            file_entry.name = entry.path().filename().string();
            file_entry.path = entry.path().string();
            file_entry.res_path = to_res_path(file_entry.path);
            file_entry.is_directory = entry.is_directory();
            
            if (!file_entry.is_directory) {
                file_entry.extension = get_file_extension(file_entry.name);
                try {
                    file_entry.file_size = std::filesystem::file_size(entry.path());
                } catch (...) {
                    file_entry.file_size = 0;
                }
            } else {
                file_entry.file_size = 0;
            }
            
            entries.push_back(file_entry);
        }
        
        // Sort entries: directories first, then files alphabetically
        std::sort(entries.begin(), entries.end(), 
                  [](const FileEntry& a, const FileEntry& b) {
                      if (a.is_directory != b.is_directory) {
                          return a.is_directory > b.is_directory;
                      }
                      return a.name < b.name;
                  });
        
        for (const auto& entry : entries) {
            render_file_entry(entry, depth);
        }
        
    } catch (const std::exception& e) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Error: %s", e.what());
    }
}

bool FileTree::render_file_entry(const FileEntry& entry, int depth) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | 
                               ImGuiTreeNodeFlags_OpenOnDoubleClick |
                               ImGuiTreeNodeFlags_SpanAvailWidth;
    
    if (!entry.is_directory) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    
    bool is_selected = (selected_file && selected_file->path == entry.path);
    if (is_selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    
    // Add indentation for depth
    for (int i = 0; i < depth; ++i) {
        ImGui::Indent(12.0f);
    }
    
    // Get icon based on file type
    const char* icon = get_file_icon(entry.extension);
    
    bool opened = false;
    if (entry.is_directory) {
        opened = ImGui::TreeNodeEx(entry.name.c_str(), flags, "%s %s", icon, entry.name.c_str());
    } else {
        opened = ImGui::TreeNodeEx(entry.name.c_str(), flags, "%s %s", icon, entry.name.c_str());
    }
    
    // Handle selection
    if (ImGui::IsItemClicked()) {
        // Create a copy of the entry to store as selected
        static FileEntry selected_entry_storage;
        selected_entry_storage = entry;
        selected_file = &selected_entry_storage;
    }
    
    // Handle double-click for file activation
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
        if (!entry.is_directory && file_activated_callback) {
            file_activated_callback(entry);
        }
    }
    
    // Show tooltip with additional info
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Path: %s", entry.res_path.c_str());
        if (!entry.is_directory) {
            ImGui::Text("Size: %.2f KB", entry.file_size / 1024.0f);
            ImGui::Text("Type: %s", entry.extension.c_str());
        }
        ImGui::EndTooltip();
    }
    
    // If directory is opened, render its contents
    if (opened && entry.is_directory) {
        render_directory_tree(entry.path, depth + 1);
    }
    
    // Always call TreePop if TreeNodeEx returned true
    if (opened) {
        ImGui::TreePop();
    }
    
    // Remove indentation
    for (int i = 0; i < depth; ++i) {
        ImGui::Unindent(12.0f);
    }
    
    return opened;
}

const char* FileTree::get_file_icon(const std::string& extension) const {
    if (extension == ".map") {
        return "[MAP]"; // Map icon
    } else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || 
               extension == ".gif" || extension == ".bmp" || extension == ".tga") {
        return "[IMG]"; // Image icon
    } else if (extension == ".txt" || extension == ".md") {
        return "[TXT]"; // Document icon
    } else if (extension == ".json") {
        return "[JSON]"; // JSON/Code icon
    } else if (extension == ".wav" || extension == ".mp3" || extension == ".ogg") {
        return "[SND]"; // Audio icon
    } else if (extension == ".obj" || extension == ".fbx" || extension == ".dae") {
        return "[3D]"; // 3D model icon
    } else if (extension.empty()) {
        return "[DIR]"; // Folder icon
    } else {
        return "[FILE]"; // Generic file icon
    }
}

std::string FileTree::get_file_extension(const std::string& filename) const {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        std::string ext = filename.substr(dot_pos);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext;
    }
    return "";
}

bool FileTree::is_supported_asset(const std::string& extension) const {
    static const std::vector<std::string> supported_extensions = {
        ".map", ".png", ".jpg", ".jpeg", ".gif", ".bmp", ".tga",
        ".wav", ".mp3", ".ogg", ".obj", ".fbx", ".dae",
        ".txt", ".json", ".md"
    };
    
    return std::find(supported_extensions.begin(), supported_extensions.end(), extension) 
           != supported_extensions.end();
}
