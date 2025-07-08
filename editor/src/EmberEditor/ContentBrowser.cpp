#include <EmberEditor/ContentBrowser.hpp>

#include <iostream>
#include <Ember.hpp>

namespace Ember
{
    ContentBrowser& ContentBrowser::get_singleton()
    {
        static ContentBrowser singleton;
        return singleton;
    }

    void ContentBrowser::on_create(const std::string& path)
    {
        this->root_path = path;
        this->current_path = path;

        this->open_new_folder_popup = false;
    }

    void ContentBrowser::on_render()
    {
        if(this->open_new_folder_popup) {
            ImGui::OpenPopup("New Folder");
            if (ImGui::BeginPopupModal("New Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                static char folder_name[256] = "";
                ImGui::InputText("Folder Name", folder_name, IM_ARRAYSIZE(folder_name));
                if (ImGui::Button("Create")) {
                    if (strlen(folder_name) > 0) {
                        std::filesystem::create_directory(this->current_path / folder_name);
                    }
                    folder_name[0] = '\0';
                    this->open_new_folder_popup = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    folder_name[0] = '\0';
                    this->open_new_folder_popup = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::Begin("Content Browser");
            // Back Button
            if(this->current_path != this->root_path) {
                if(ImGui::Button("..")) {
                    this->current_path = this->current_path.parent_path();
                }
            } else {
                ImGui::BeginDisabled();
                ImGui::Button("..");
                ImGui::EndDisabled();
            }
            
            // Background context menu (only when not clicking on an item)
            if (ImGui::BeginPopupContextWindow("ContentBrowserContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
                if (ImGui::MenuItem("Refresh")) {
                    // Implement refresh logic if needed
                }
                if (ImGui::MenuItem("New Folder")) {
                    // Set a flag to open the popup outside the menu processing
                    ImGui::SetItemDefaultFocus();
                    ImGui::CloseCurrentPopup();
                    this->open_new_folder_popup = true;
                }
                if (ImGui::MenuItem("Import Asset")) {
                    // Implement import asset logic if needed
                }
                ImGui::EndPopup();
            }

            float padding = 16.0f;
            float thumbnail_size = 64.0f;
            float cell_size = thumbnail_size + padding;

            float panel_width = ImGui::GetContentRegionAvail().x;
            int column_count = (int)(panel_width / cell_size);
            if(column_count < 1) {
                column_count = 1;
            }

            ImGui::Columns(column_count, 0, false);

                for(auto& entry : std::filesystem::directory_iterator(this->current_path)) {
                    const auto& path = entry.path();
                    std::string name = path.filename().string();

                    ImGui::PushID(name.c_str());
                    
                    // Different icons for files and folders
                    const char* icon = entry.is_directory() ? "📁" : "📄";
                    ImGui::Button(icon, ImVec2(thumbnail_size, thumbnail_size));

                    // Drag source - allow dragging any item
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        // Set payload to carry the full path of the item being dragged
                        std::string payload_data = path.string();
                        ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", payload_data.c_str(), payload_data.size() + 1);
                        
                        // Display preview while dragging
                        ImGui::Text("%s %s", icon, name.c_str());
                        ImGui::EndDragDropSource();
                    }

                    // Drop target - only allow dropping on folders
                    if (entry.is_directory() && ImGui::BeginDragDropTarget()) {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                            std::string source_path((const char*)payload->Data);
                            std::filesystem::path source(source_path);
                            std::filesystem::path destination = path / source.filename();
                            
                            // Prevent dropping a folder into itself or its subdirectories
                            bool is_valid_drop = true;
                            if (std::filesystem::is_directory(source)) {
                                std::filesystem::path relative_path = std::filesystem::relative(path, source);
                                if (relative_path.string().find("..") == std::string::npos) {
                                    is_valid_drop = false; // Trying to move folder into its own subdirectory
                                }
                            }
                            
                            if (is_valid_drop && source != destination) {
                                try {
                                    // Check if destination already exists
                                    if (!std::filesystem::exists(destination)) {
                                        std::filesystem::rename(source, destination);
                                    } else {
                                        // Handle collision - could show a dialog here
                                        // For now, just skip the operation
                                    }
                                } catch (const std::filesystem::filesystem_error& e) {
                                    // Handle error - could show an error message
                                }
                            }
                        }
                        ImGui::EndDragDropTarget();
                    }

                    // Item context menu
                    if (ImGui::BeginPopupContextItem(("ItemContextMenu" + name).c_str())) {
                        if (entry.is_directory()) {
                            // Folder context menu
                            if (ImGui::MenuItem("Open")) {
                                this->current_path /= path.filename();
                            }
                            if (ImGui::MenuItem("Rename")) {
                                // Implement rename logic for folders
                            }
                            if (ImGui::MenuItem("Delete")) {
                                // Implement delete logic for folders
                            }
                        } else {
                            // File context menu
                            if (ImGui::MenuItem("Open")) {
                                this->selected_file = path.string();
                                // Implement file opening logic
                            }
                            if (ImGui::MenuItem("Rename")) {
                                // Implement rename logic for files
                            }
                            if (ImGui::MenuItem("Delete")) {
                                // Implement delete logic for files
                            }
                            ImGui::Separator();
                            if (ImGui::MenuItem("Show in Explorer")) {
                                // Implement show in file explorer logic
                            }
                            if (ImGui::MenuItem("Properties")) {
                                // Implement properties dialog logic
                            }
                        }
                        ImGui::EndPopup();
                    }

                    if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        if(entry.is_directory()) {
                            this->current_path /= path.filename();
                        } else {
                            this->selected_file = path.string();
                        }
                    }

                    ImGui::TextWrapped("%s", name.c_str());

                    ImGui::NextColumn();
                    ImGui::PopID();
                }

            ImGui::Columns(1);

        ImGui::End();
    }
}