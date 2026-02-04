#pragma once

#include <string>

namespace EmberEngine
{
    class Display
    {
        private:
            unsigned int target_fps;
        public:
            static Display& get_singleton();
        public:
            static void create(unsigned int width, unsigned int height, const std::string& title);
            static void destroy();
            static bool should_close();
        public: // --- Setters --- //
            static void set_size(unsigned int width, unsigned int height);
            static void set_target_fps(unsigned int target_fps);
    };
}