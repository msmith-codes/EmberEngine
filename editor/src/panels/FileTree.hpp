#pragma once

#include <imgui.h>
#include <string>
#include <vector>
#include <filesystem>
#include <functional>

struct FileEntry {
    std::string name;
    std::string path;  // Full path
    std::string res_path;  // res:// protocol path
    bool is_directory;
    std::string extension;
    size_t file_size;
};

class FileTree {
public:
    FileTree();
    ~FileTree();
    
    // Main render function for the panel
    void render_ui();
    
    // Set the root directory for the file explorer
    void set_root_directory(const std::string& path);
    
    // Get the currently selected file
    const FileEntry* get_selected_file() const;
    
    // Convert filesystem path to res:// protocol path
    std::string to_res_path(const std::string& filesystem_path) const;
    
    // Convert res:// protocol path to filesystem path
    std::string from_res_path(const std::string& res_path) const;
    
    // Set callback for when a file is double-clicked
    void set_file_activated_callback(std::function<void(const FileEntry&)> callback);
    
    // Refresh the file tree
    void refresh();
    
private:
    std::string root_directory;
    std::vector<FileEntry> current_entries;
    FileEntry* selected_file;
    std::function<void(const FileEntry&)> file_activated_callback;
    
    // Scan directory and populate entries
    void scan_directory(const std::string& directory_path);
    
    // Render file tree recursively
    void render_directory_tree(const std::string& directory_path, int depth = 0);
    
    // Render a single file entry
    bool render_file_entry(const FileEntry& entry, int depth);
    
    // Get icon for file type
    const char* get_file_icon(const std::string& extension) const;
    
    // Helper to get file extension
    std::string get_file_extension(const std::string& filename) const;
    
    // Check if path is a supported asset type
    bool is_supported_asset(const std::string& extension) const;
};

