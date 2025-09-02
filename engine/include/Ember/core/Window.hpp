#pragma once

#include <string>

namespace Ember
{
    class Window
    {
        public:
            static Window& get_singleton();
        private:
            std::string title;
            unsigned int width;
            unsigned int height;
        private:
            Window();
        public:
            ~Window();
        public: // -- Static Methods -- //
            static void create(const std::string& title, unsigned int width, unsigned int height);
            static void shutdown();
            static bool should_close();
        public: // -- Static Getters -- //
            static std::string& get_title();
            static unsigned int get_width();
            static unsigned int get_height();
        public: // -- Static Setters -- //
            static void set_title(const std::string& title);
            static void set_size(unsigned int width, unsigned int height);


    };
}

