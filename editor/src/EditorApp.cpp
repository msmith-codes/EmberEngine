#include <EmberEngine/EmberEngine.hpp>
#include <EmberEngine/EntryPoint.hpp>
#include <EmberEngine/core/Logger.hpp>

#include <panels/EditorViewport2D.hpp>
#include <panels/EditorViewport3D.hpp>
#include <panels/InspectorPanel.hpp>
#include <panels/FileTree.hpp>

#include <nfd.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{
    struct ProjectConfig
    {
        std::string project_name;
        std::string display_title;
        int display_width = 1280;
        int display_height = 720;
        std::string start_scene;
    };

    std::string trim(const std::string& input)
    {
        size_t start = 0;
        while(start < input.size() && std::isspace(static_cast<unsigned char>(input[start]))) {
            ++start;
        }
        size_t end = input.size();
        while(end > start && std::isspace(static_cast<unsigned char>(input[end - 1]))) {
            --end;
        }
        return input.substr(start, end - start);
    }

    std::string to_lower(const std::string& input)
    {
        std::string out = input;
        std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return out;
    }

    bool parse_display_size(const std::string& value, int& out_width, int& out_height)
    {
        std::string sanitized = value;
        for(char& c : sanitized) {
            if(c == 'x' || c == 'X' || c == ',' || c == ';') {
                c = ' ';
            }
        }

        std::istringstream stream(sanitized);
        int width = 0;
        int height = 0;
        if(!(stream >> width >> height)) {
            return false;
        }

        if(width <= 0 || height <= 0) {
            return false;
        }

        out_width = width;
        out_height = height;
        return true;
    }

    bool load_project_file(const std::string& path, ProjectConfig& config, std::string& error)
    {
        std::ifstream file(path);
        if(!file.is_open()) {
            error = "Unable to open project file.";
            return false;
        }

        std::string line;
        while(std::getline(file, line)) {
            std::string trimmed = trim(line);
            if(trimmed.empty()) {
                continue;
            }
            if(trimmed.rfind("#", 0) == 0 || trimmed.rfind("//", 0) == 0) {
                continue;
            }

            size_t sep = trimmed.find('=');
            if(sep == std::string::npos) {
                continue;
            }

            std::string key = to_lower(trim(trimmed.substr(0, sep)));
            std::string value = trim(trimmed.substr(sep + 1));

            if(key == "project_name") {
                config.project_name = value;
            } else if(key == "display_title") {
                config.display_title = value;
            } else if(key == "display_size") {
                int width = config.display_width;
                int height = config.display_height;
                if(!parse_display_size(value, width, height)) {
                    error = "Invalid display_size format.";
                    return false;
                }
                config.display_width = width;
                config.display_height = height;
            } else if(key == "start_scene") {
                config.start_scene = value;
            }
        }

        if(config.project_name.empty()) {
            config.project_name = "Untitled";
        }
        if(config.display_title.empty()) {
            config.display_title = config.project_name;
        }

        return true;
    }

    bool open_project_dialog(ProjectConfig& config, std::string& out_path, std::string& error)
    {
        const nfdfilteritem_t filters[1] = { { "Ember Project", "ember" } };
        nfdchar_t* out_path_raw = nullptr;

        nfdresult_t result = NFD_OpenDialog(&out_path_raw, filters, 1, nullptr);
        if(result == NFD_OKAY) {
            std::string path = out_path_raw ? out_path_raw : "";
            if(out_path_raw) {
                NFD_FreePath(out_path_raw);
            }

            ProjectConfig loaded_config;
            std::string load_error;
            if(!load_project_file(path, loaded_config, load_error)) {
                error = load_error;
                return false;
            }

            config = loaded_config;
            out_path = path;
            error.clear();
            return true;
        }

        if(result == NFD_CANCEL) {
            error = "Project selection canceled.";
            return false;
        }

        const char* nfd_error = NFD_GetError();
        error = nfd_error ? nfd_error : "NFD failed to open the dialog.";
        return false;
    }

    bool save_project_file(const std::string& path, const ProjectConfig& config, std::string& error)
    {
        std::ofstream file(path, std::ios::out | std::ios::trunc);
        if(!file.is_open()) {
            error = "Unable to create project file.";
            return false;
        }

        file << "project_name=" << config.project_name << "\n";
        file << "display_title=" << config.display_title << "\n";
        file << "display_size=" << config.display_width << "x" << config.display_height << "\n";
        file << "start_scene=" << config.start_scene << "\n";

        if(!file.good()) {
            error = "Failed writing project file.";
            return false;
        }

        return true;
    }

    bool create_project_dialog(ProjectConfig& config, std::string& out_path, std::string& error)
    {
        const nfdfilteritem_t filters[1] = { { "Ember Project", "ember" } };
        nfdchar_t* out_path_raw = nullptr;

        nfdresult_t result = NFD_SaveDialog(&out_path_raw, filters, 1, nullptr, "project.ember");
        if(result == NFD_OKAY) {
            std::string path = out_path_raw ? out_path_raw : "";
            if(out_path_raw) {
                NFD_FreePath(out_path_raw);
            }

            ProjectConfig new_config;
            new_config.project_name = "New Project";
            new_config.display_title = "New Project";
            new_config.display_width = 1280;
            new_config.display_height = 720;
            new_config.start_scene = "Main";

            std::string save_error;
            if(!save_project_file(path, new_config, save_error)) {
                error = save_error;
                return false;
            }

            config = new_config;
            out_path = path;
            error.clear();
            return true;
        }

        if(result == NFD_CANCEL) {
            error = "Project creation canceled.";
            return false;
        }

        const char* nfd_error = NFD_GetError();
        error = nfd_error ? nfd_error : "NFD failed to open the dialog.";
        return false;
    }
}

class EditorApp : public EmberEngine::Application
{
    private:
        EditorViewport2D* viewport_2d;
        EditorViewport3D* viewport_3d;
        InspectorPanel* inspector;
        FileTree* file_tree;
        ProjectConfig project_config;
        bool project_loaded;
        bool project_prompt_requested;
        bool project_popup_open;
        std::string project_path;
        std::string project_error;
    protected:
        void on_compose() override
        {
            EmberEngine::Display::set_resizable(true);
            EmberEngine::Display::create(1280, 720, "EmberEditor (v0.1.0)");
            EmberEngine::Display::set_target_fps(60);

            if(NFD_Init() != NFD_OKAY) {
                const char* nfd_error = NFD_GetError();
                EmberEngine::Logger::error("EditorApp", nfd_error ? nfd_error : "Failed to initialize NFD.");
            }
        
            this->viewport_2d = new EditorViewport2D();
            this->viewport_3d = new EditorViewport3D();
            this->inspector = new InspectorPanel();
            this->file_tree = new FileTree();
            
            // Set up file loading callback
            this->file_tree->set_file_activated_callback([this](const FileEntry& entry) {
                if (entry.extension == ".map") {
                    // Load map file
                    EmberEngine::Logger::info("EditorApp", "Loading map: " + entry.res_path);
                    
                    // Convert res:// path back to filesystem path
                    std::string filesystem_path = this->file_tree->from_res_path(entry.res_path);
                    
                    // Load the map into the 2D viewport
                    if (this->viewport_2d->load_map(filesystem_path)) {
                        EmberEngine::Logger::info("EditorApp", "Successfully loaded map: " + entry.name);
                    } else {
                        EmberEngine::Logger::error("EditorApp", "Failed to load map: " + entry.name);
                    }
                } else {
                    EmberEngine::Logger::info("EditorApp", "File selected: " + entry.res_path);
                }
            });

            this->project_loaded = false;
            this->project_prompt_requested = true;
            this->project_popup_open = false;
        }

        void on_dispose() override
        {
            delete this->viewport_2d;
            delete this->viewport_3d;
            delete this->inspector;
            delete this->file_tree;

            NFD_Quit();

            EmberEngine::Display::destroy();
        }

        void on_update(float delta) override
        {

        }

        void on_render() override
        {
            ImGui::DockSpaceOverViewport(0,  NULL, ImGuiDockNodeFlags_PassthruCentralNode);

            if(!this->project_loaded) {
                if(this->project_prompt_requested) {
                    this->project_prompt_requested = false;
                    this->project_popup_open = true;
                    ImGui::OpenPopup("Project Setup");
                }

                if(this->project_popup_open) {
                    ImGui::SetNextWindowSize(ImVec2(420, 0), ImGuiCond_Appearing);
                    if(ImGui::BeginPopupModal("Project Setup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                        ImGui::Text("Open an existing project or create a new one.");
                        ImGui::Spacing();

                        if(!this->project_error.empty()) {
                            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "%s", this->project_error.c_str());
                            ImGui::Spacing();
                        }

                        if(ImGui::Button("Open Project", ImVec2(160.0f, 0.0f))) {
                            if(open_project_dialog(this->project_config, this->project_path, this->project_error)) {
                                this->project_loaded = true;
                                this->project_popup_open = false;
                                ImGui::CloseCurrentPopup();
                                EmberEngine::Display::set_title(this->project_config.display_title);
                                EmberEngine::Display::set_size(this->project_config.display_width, this->project_config.display_height);
                                
                                // Update FileTree root to project directory
                                std::filesystem::path project_file_path(this->project_path);
                                std::string project_dir = project_file_path.parent_path().string();
                                if (project_dir.empty()) project_dir = ".";
                                this->file_tree->set_root_directory(project_dir);
                                
                                EmberEngine::Logger::info("EditorApp", "Loaded project: " + this->project_path);
                            } else if(!this->project_error.empty()) {
                                EmberEngine::Logger::warn("EditorApp", this->project_error);
                            }
                        }
                        ImGui::SameLine();
                        if(ImGui::Button("Create Project", ImVec2(160.0f, 0.0f))) {
                            if(create_project_dialog(this->project_config, this->project_path, this->project_error)) {
                                this->project_loaded = true;
                                this->project_popup_open = false;
                                ImGui::CloseCurrentPopup();
                                EmberEngine::Display::set_title(this->project_config.display_title);
                                EmberEngine::Display::set_size(this->project_config.display_width, this->project_config.display_height);
                                
                                // Update FileTree root to project directory
                                std::filesystem::path project_file_path(this->project_path);
                                std::string project_dir = project_file_path.parent_path().string();
                                if (project_dir.empty()) project_dir = ".";
                                this->file_tree->set_root_directory(project_dir);
                                
                                EmberEngine::Logger::info("EditorApp", "Created project: " + this->project_path);
                            } else if(!this->project_error.empty()) {
                                EmberEngine::Logger::warn("EditorApp", this->project_error);
                            }
                        }

                        ImGui::EndPopup();
                    }
                }

                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
                ImGui::Begin("Project Required", nullptr, flags);

                ImVec2 available = ImGui::GetContentRegionAvail();
                ImVec2 text_size = ImGui::CalcTextSize("Select a project to continue.");
                ImGui::SetCursorPosX(std::max(0.0f, (available.x - text_size.x) * 0.5f));
                ImGui::Text("Select a project to continue.");

                if(!this->project_error.empty()) {
                    ImGui::Spacing();
                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "%s", this->project_error.c_str());
                }

                ImGui::Spacing();
                ImGui::SetCursorPosX(std::max(0.0f, (available.x - 160.0f) * 0.5f));
                if(ImGui::Button("Select Project", ImVec2(160.0f, 0.0f))) {
                    this->project_prompt_requested = true;
                }

                ImGui::End();
                return;
            }

            // --- RENDER VIEWPORT --- //
            this->viewport_2d->begin_render();
            // TODO: Render Scene here.
            this->viewport_2d->end_render();

            // Set 3D viewport data before rendering
            this->viewport_3d->set_walls(&this->viewport_2d->get_walls());
            this->viewport_3d->set_rooms(&this->viewport_2d->get_rooms());
            
            this->viewport_3d->begin_render();
            this->viewport_3d->end_render();

            this->viewport_3d->render_ui();
            this->viewport_2d->render_ui();
            this->inspector->render_ui(&this->viewport_2d->get_walls_mutable(), this->viewport_2d->get_selected_wall_index(), &this->viewport_2d->get_rooms_mutable(), this->viewport_2d->get_selected_room_index());
            this->file_tree->render_ui();
        }

        bool is_running() override
        {
            return (!EmberEngine::Display::should_close());
        }

};

EmberEngine::Application* EmberEngine::create_application()
{
    return new EditorApp();
}