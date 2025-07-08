#ifndef EE_ContentBrowser_HPP
#define EE_ContentBrowser_HPP

#include <string>
#include <vector>
#include <filesystem>

namespace Ember
{
    class ContentBrowser
    {
        public:
            static ContentBrowser& get_singleton();
        private:
            std::filesystem::path root_path;
            std::filesystem::path current_path;
            std::string selected_file;
        private:
            bool open_new_folder_popup;
        public:
            void on_create(const std::string& path);
            void on_render();
    };
}

#endif